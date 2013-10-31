# Ruby GeoIP2 Bindings

## Description

Detects city by ip address by local database from [maxmind.com](http://dev.maxmind.com/geoip/geoip2/geolite2/)

## Installation

[libmaxminddb](https://github.com/maxmind/libmaxminddb) must be installed.

```
gem 'geoip2', github: 'da4nik/geoip2'
```

## Usage

Returns nil if nothing found and raises exception if file not opened

```ruby
require 'GeoIP2'

city_name = GeoIP2::locate_city(<local_db_file.mmdb>, <ip address>, <optional lang>)

city_name = GeoIP2::locate_city('GeoLite2-City.mmdb', '77.93.127.33')
=> Тамбов

city_name = GeoIP2::locate_city('GeoLite2-City.mmdb', '77.93.127.33', 'en')
=> Tambov
```

