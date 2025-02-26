#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Code get from https://toster.ru/q/72866

import os, sys, json
import urllib.parse as ul

def download_file_from_yandex_disk(download_url, save_directory):
    base_url = 'https://cloud-api.yandex.net:443/v1/disk/public/resources/download?public_key='
    url = ul.quote_plus(download_url)
    print("!", 'wget -qO - {}{}'.format(base_url, url))
    res = os.popen('wget -qO - {}{}'.format(base_url, url)).read()
    json_res = json.loads(res)
    filename = ul.parse_qs(ul.urlparse(json_res['href']).query)['filename'][0]
    os.system("wget '{}' -P '{}' -O '{}'".format(json_res['href'], save_directory, filename))

if __name__ == "__main__":
    
    urls = [
        "https://disk.yandex.ru/d/JUb4Ywvfnn_QEg",
        "https://disk.yandex.ru/d/s17PWdErohIdgQ",
        "https://disk.yandex.ru/d/UkKlJDns0yJdfg",
    ]
    
    for download_url in urls:
        download_file_from_yandex_disk(download_url, ".")

    