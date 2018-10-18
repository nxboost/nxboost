
Debian
====================
This directory contains files used to package nxboostd/nxboost-qt
for Debian-based Linux systems. If you compile nxboostd/nxboost-qt yourself, there are some useful files here.

## nxboost: URI support ##


nxboost-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install nxboost-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your nxboostqt binary to `/usr/bin`
and the `../../share/pixmaps/nxboost128.png` to `/usr/share/pixmaps`

nxboost-qt.protocol (KDE)

