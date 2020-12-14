#!/bin/sh
proxy=$1
cp /tmp/$proxy ~/
export X509_USER_PROXY=~/$proxy
