// Copyright (c) 2017-2018 The PIVX developers
// Copyright (c) 2018-2019 The NXBoost developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef zNXBCONTROLDIALOG_H
#define zNXBCONTROLDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "primitives/zerocoin.h"
#include "privacydialog.h"

class CZerocoinMint;
class WalletModel;

namespace Ui {
class zNXBControlDialog;
}

class CZNXBControlWidgetItem : public QTreeWidgetItem
{
public:
    explicit CZNXBControlWidgetItem(QTreeWidget *parent, int type = Type) : QTreeWidgetItem(parent, type) {}
    explicit CZNXBControlWidgetItem(int type = Type) : QTreeWidgetItem(type) {}
    explicit CZNXBControlWidgetItem(QTreeWidgetItem *parent, int type = Type) : QTreeWidgetItem(parent, type) {}

    bool operator<(const QTreeWidgetItem &other) const;
};

class ZNXBControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit zNXBControlDialog(QWidget *parent);
    ~zNXBControlDialog();

    void setModel(WalletModel* model);

    static std::set<std::string> setSelectedMints;
    static std::set<CMintMeta> setMints;
    static std::vector<CMintMeta> GetSelectedMints();

private:
    Ui::zNXBControlDialog *ui;
    WalletModel* model;
    PrivacyDialog* privacyDialog;

    void updateList();
    void updateLabels();

    enum {
        COLUMN_CHECKBOX,
        COLUMN_DENOMINATION,
        COLUMN_PUBCOIN,
        COLUMN_VERSION,
        COLUMN_CONFIRMATIONS,
        COLUMN_ISSPENDABLE
    };
    friend class CZNXBControlWidgetItem;

private slots:
    void updateSelection(QTreeWidgetItem* item, int column);
    void ButtonAllClicked();
};

#endif // zNXBCONTROLDIALOG_H
