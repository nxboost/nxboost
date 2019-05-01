// Copyright (c) 2017-2018 The PIVX developers
// Copyright (c) 2018-2019 The NXBoost developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Denominations.h"
#include "libzerocoin/Coin.h"
#include "amount.h"
#include "chainparams.h"
#include "coincontrol.h"
#include "main.h"
#include "wallet/wallet.h"
#include "wallet/walletdb.h"
#include "txdb.h"
#include "znxb/znxbmodule.h"
#include <boost/test/unit_test.hpp>
#include <iostream>

using namespace libzerocoin;


BOOST_AUTO_TEST_SUITE(zerocoin_transactions_tests)

static CWallet cWallet("unlocked.dat");

BOOST_AUTO_TEST_CASE(zerocoin_spend_test)
{
    SelectParams(CBaseChainParams::MAIN);
    ZerocoinParams *ZCParams = Params().Zerocoin_Params(false);
    (void)ZCParams;

    bool fFirstRun;
    cWallet.LoadWallet(fFirstRun);
    cWallet.znxbTracker = unique_ptr<CzNXBTracker>(new CzNXBTracker(cWallet.strWalletFile));
    CMutableTransaction tx;
    CWalletTx* wtx = new CWalletTx(&cWallet, tx);
    bool fMintChange=true;
    bool fMinimizeChange=true;
    std::vector<CZerocoinSpend> vSpends;
    std::vector<CZerocoinMint> vMints;
    CAmount nAmount = COIN;

    CZerocoinSpendReceipt receipt;
    cWallet.SpendZerocoin(nAmount, *wtx, receipt, vMints, fMintChange, fMinimizeChange);

    BOOST_CHECK_MESSAGE(receipt.GetStatus() == ZNXB_TRX_FUNDS_PROBLEMS, "Failed Invalid Amount Check");

    nAmount = 1;
    CZerocoinSpendReceipt receipt2;
    cWallet.SpendZerocoin(nAmount, *wtx, receipt2, vMints, fMintChange, fMinimizeChange);

    // if using "wallet.dat", instead of "unlocked.dat" need this
    /// BOOST_CHECK_MESSAGE(vString == "Error: Wallet locked, unable to create transaction!"," Locked Wallet Check Failed");

    BOOST_CHECK_MESSAGE(receipt2.GetStatus() == ZNXB_TRX_FUNDS_PROBLEMS, "Failed Invalid Amount Check");

}

BOOST_AUTO_TEST_CASE(zerocoin_public_spend_test)
{
    SelectParams(CBaseChainParams::MAIN);
    ZerocoinParams *ZCParams = Params().Zerocoin_Params(false);
    (void)ZCParams;

    PrivateCoin privCoin(ZCParams, libzerocoin::CoinDenomination::ZQ_ONE, true);
    const CPrivKey privKey = privCoin.getPrivKey();

    CZerocoinMint mint = CZerocoinMint(
            privCoin.getPublicCoin().getDenomination(),
            privCoin.getPublicCoin().getValue(),
            privCoin.getRandomness(),
            privCoin.getSerialNumber(),
            false,
            privCoin.getVersion(),
            nullptr);
    mint.SetPrivKey(privKey);


    // Mint tx
    CTransaction prevTx;

    CScript scriptSerializedCoin = CScript()
    << OP_ZEROCOINMINT << privCoin.getPublicCoin().getValue().getvch().size() << privCoin.getPublicCoin().getValue().getvch();
    CTxOut out = CTxOut(libzerocoin::ZerocoinDenominationToAmount(privCoin.getPublicCoin().getDenomination()), scriptSerializedCoin);
    prevTx.vout.push_back(out);

    mint.SetOutputIndex(0);
    mint.SetTxHash(prevTx.GetHash());

    // Spend tx
    CMutableTransaction tx;
    tx.vout.resize(1);
    tx.vout[0].nValue = 1*CENT;
    tx.vout[0].scriptPubKey = GetScriptForDestination(CBitcoinAddress("D9Ti4LEhF1n6dR2hGd2SyNADD51AVgva6q").Get());

    CTxIn in;
    if (!ZNXBModule::createInput(in, mint, tx.GetHash())){
        BOOST_CHECK_MESSAGE(false, "Failed to create zc input");
    }

    PublicCoinSpend publicSpend(ZCParams);
    if (!ZNXBModule::validateInput(in, out, tx, publicSpend)){
        BOOST_CHECK_MESSAGE(false, "Failed to validate zc input");
    }

    PublicCoinSpend publicSpendTest(ZCParams);
    BOOST_CHECK_MESSAGE(ZNXBModule::parseCoinSpend(in, tx, out, publicSpendTest), "Failed to parse public spend");
    libzerocoin::CoinSpend *spend = &publicSpendTest;

    BOOST_CHECK_MESSAGE(publicSpendTest.HasValidSignature(), "Failed to validate public spend signature");
    BOOST_CHECK_MESSAGE(spend->HasValidSignature(), "Failed to validate spend signature");

}

BOOST_AUTO_TEST_SUITE_END()
