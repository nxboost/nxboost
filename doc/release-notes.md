NXBoost Core version *4.4.0* is now available from:  <https://github.com/nxboost/nxboost/releases>

This is a new major version release, including various bug fixes and performance improvements, as well as updated translations.

Please report bugs using the issue tracker at github: <https://github.com/nxboost/nxboost/issues>


Mandatory Update
==============

NXBoost Core v4.4.0 is a mandatory update for all users. This release contains new consensus rules and improvements that are not backwards compatible with older versions. Users will have a grace period of approximately one week to update their clients before enforcement of this update goes into effect.

Masternodes will need to be restarted once both the masternode daemon and the controller wallet have been upgraded.

How to Upgrade
==============

If you are running an older version, shut it down. Wait until it has completely shut down (which might take a few minutes for older versions), then run the installer (on Windows) or just copy over /Applications/NXBoost-Qt (on Mac) or nxboostd/nxboost-qt (on Linux).


Compatibility
==============

NXBoost Core is extensively tested on multiple operating systems using the Linux kernel, macOS 10.10+, and Windows 7 and later.

Microsoft ended support for Windows XP on [April 8th, 2014](https://www.microsoft.com/en-us/WindowsForBusiness/end-of-xp-support), No attempt is made to prevent installing or running the software on Windows XP, you can still do so at your own risk but be aware that there are known instabilities and issues. Please do not report issues about Windows XP to the issue tracker.

Apple released it's last Mountain Lion update August 13, 2015, and officially ended support on [December 14, 2015](http://news.fnal.gov/2015/10/mac-os-x-mountain-lion-10-8-end-of-life-december-14/). NXBoost Core software starting with v4.3.0 will no longer run on MacOS versions prior to Yosemite (10.10). Please do not report issues about MacOS versions prior to Yosemite to the issue tracker.

NXBoost Core should also work on most other Unix-like systems but is not frequently tested on them.


Notable Changes
==============

## zNXB Public Spends

Recent exploits of the Zerocoin protocol (Wrapped serials and broken P1 proof) required us to enable the zerocoin spork and deactivate zNXB functionality in order to secure the supply until the pertinent review process was completed.

Moving forward from this undesired situation, we are enabling a secure and chain storage friendly solution for the zerocoin public spend (aka zNXB to NXB conversion).

The explanation of how this works can be found in #891

After block `450,000` has past, `SPORK_16` will be deactivated to allow zNXB spends to occur using this new public spend method for version 2 zNXB (version 1 zNXB won't be spendable, see note below). zNXB public spends, as the name suggests, are **NOT** private, they reveal the input mint that is being spent. The minting of **NEW** zNXB, as well as zNXB staking will remain disabled for the time being.

It is advised that users spend/convert their existing zNXB to NXB, which can be done via the GUI or RPC as it was prior to the disabling of zNXB. Note that with the public spend method, the restriction on the number of denominations per transaction (previously 7) has been lifted, and now allows for several hundred denominations per transaction.

## GUI Changes

### Options Dialog Cleanup

The options/settings UI dialog has been cleaned up to no longer show settings that are wallet related when running in "disable wallet" (`-disablewallet`) mode.

### Privacy Tab

Notice text has been added to the privacy tab indicating that zNXB minting is disabled, as well as the removal of UI elements that supported such functionality. Notice text has also been added indicating that zNXB spends are currently **NOT** private.

## RPC Changes

### Removal of Deprecated Commands

The `masternode` and `mnbudget` RPC commands, which were marked as deprecated in PIVX Core v2.3.1 (September 19, 2017), have now been completely removed from NXBoos Core.

Several new commands were added in v2.3.1 to replace the two aforementioned commands, reference the [v2.3.1 Release Notes](https://github.com/PIVX-Project/PIVX/blob/master/doc/release-notes/release-notes-2.3.1.md#rpc-changes) for further details.

### New `getblockindexstats` Command

A new RPC command (`getblockindexstats`) has been introduced which serves the purpose of obtaining statistical information on a range of blocks. The information returned is as follows:
  * transaction count (not including coinbase/coinstake txes)
  * transaction count (including coinbase/coinstake txes)
  * zNXB per-denom mint count
  * zNXB per-denom spend count
  * total transaction bytes
  * total fees in block range
  * average fee per kB

Command Reference:
```$xslt
getblockindexstats height range ( fFeeOnly )
nReturns aggregated BlockIndex data for blocks
height, height+1, height+2, ..., height+range-1]

nArguments:
1. height             (numeric, required) block height where the search starts.
2. range              (numeric, required) number of blocks to include.
3. fFeeOnly           (boolean, optional, default=False) return only fee info.
```
Result:
```
{
  first_block: x,              (integer) First counted block
  last_block: x,               (integer) Last counted block
  txcount: xxxxx,              (numeric) tx count (excluding coinbase/coinstake)
  txcount_all: xxxxx,          (numeric) tx count (including coinbase/coinstake)
  mintcount: {              [if fFeeOnly=False]
        denom_1: xxxx,         (numeric) number of mints of denom_1 occurred over the block range
        denom_5: xxxx,         (numeric) number of mints of denom_5 occurred over the block range
         ...                    ... number of mints of other denominations: ..., 10, 50, 100, 500, 1000, 5000
  },
  spendcount: {             [if fFeeOnly=False]
        denom_1: xxxx,         (numeric) number of spends of denom_1 occurred over the block range
        denom_5: xxxx,         (numeric) number of spends of denom_5 occurred over the block range
         ...                    ... number of spends of other denominations: ..., 10, 50, 100, 500, 1000, 5000
  },
  pubspendcount: {          [if fFeeOnly=False]
        denom_1: xxxx,         (numeric) number of PUBLIC spends of denom_1 occurred over the block range
        denom_5: xxxx,         (numeric) number of PUBLIC spends of denom_5 occurred over the block range
         ...                   ... number of PUBLIC spends of other denominations: ..., 10, 50, 100, 500, 1000, 5000
  },
  txbytes: xxxxx,              (numeric) Sum of the size of all txes (zNXB excluded) over block range
  ttlfee: xxxxx,               (numeric) Sum of the fee amount of all txes (zNXB mints excluded) over block range
  ttlfee_all: xxxxx,           (numeric) Sum of the fee amount of all txes (zNXB mints included) over block range
  feeperkb: xxxxx,             (numeric) Average fee per kb (excluding zc txes)
}
```

## Build System Changes

### New Architectures for Depends

The depends system has new added support for the `s390x` and `ppc64el` architectures. This is done in order to support the future integration with [Snapcraft](https://www.snapcraft.io), as well as to support any developers who may use systems based on such architectures.

### Basic CMake Support

While the existing Autotools based build system is our standard build system, and will continue to be so, we have added basic support for compiling with CMake on macOS and linux systems.

This is intended to be used in conjunction with IDEs like CLion (which relies heavily on CMake) in order to streamline the development process. Developers can now use, for example, CLion's internal debugger and profiling tools.

Note that it is still required to have relevant dependencies installed on the system for this to function properly.

As well as everyone that helped translating on [Transifex](https://www.transifex.com/projects/nxboost/nxboost-wallet/).
