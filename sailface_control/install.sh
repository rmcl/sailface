#!/usr/bin/env bash

SAILFACE_DIR=`pwd`

sudo rm /etc/systemd/system/sailface.service
sudo cp $SAILFACE_DIR/sailface.systemd.conf /etc/systemd/system/sailface.service
sudo systemctl daemon-reload
sudo systemctl enable sailface.service
sudo systemctl status sailface.service
