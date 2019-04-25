// Copyright (c) 2017-2018 The PIVX developers
// Copyright (c) 2018-2019 The NXBoost developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ZNXBCONTROLDIALOG_H
#define ZNXBCONTROLDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "znxb/zerocoin.h"
#include "privacydialog.h"

class CZerocoinMint;
class WalletModel;

namespace Ui {
class ZNxbControlDialog;
}

class CZNXBControlWidgetItem : public QTreeWidgetItem
{
public:
    explicit CZNXBControlWidgetItem(QTreeWidget *parent, int type = Type) : QTreeWidgetItem(parent, type) {}
    explicit CZNXBControlWidgetItem(int type = Type) : QTreeWidgetItem(type) {}
    explicit CZNXBControlWidgetItem(QTreeWidgetItem *parent, int type = Type) : QTreeWidgetItem(parent, type) {}

    bool operator<(const QTreeWidgetItem &other) const;
};

class ZNxbControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ZNxbControlDialog(QWidget *parent);
    ~ZNxbControlDialog();

    void setModel(WalletModel* model);

    static std::set<std::string> setSelectedMints;
    static std::set<CMintMeta> setMints;
    static std::vector<CMintMeta> GetSelectedMints();

private:
    Ui::ZNxbControlDialog *ui;
    WalletModel* model;
    PrivacyDialog* privacyDialog;

    void updateList();
    void updateLabels();

    enum {
        COLUMN_CHECKBOX,
        COLUMN_DENOMINATION,
        COLUMN_PUBCOIN,
        COLUMN_VERSION,
        COLUMN_PRECOMPUTE,
        COLUMN_CONFIRMATIONS,
        COLUMN_ISSPENDABLE
    };
    friend class CZNXBControlWidgetItem;

private slots:
    void updateSelection(QTreeWidgetItem* item, int column);
    void ButtonAllClicked();
};

#endif // ZNXBCONTROLDIALOG_H
