# Ruby GeoIP2 Bindings

## Description

Searches city by ip address in local database from [maxmind.com](http://dev.maxmind.com/geoip/geoip2/geolite2/)

## Installation

[libmaxminddb](https://github.com/maxmind/libmaxminddb) must be installed.

```
gem 'geoip2', github: 'da4nik/geoip2'
```

## Usage

Returns nil if nothing found and raises exception if file not opened

```ruby
require 'GeoIP2'

city = GeoIP2::locate(<local_db_file.mmdb>, <ip address>, <optional lang>)

city = GeoIP2::locate('GeoLite2-City.mmdb', '77.93.127.33')
=> {"city"=>"Тамбов",
 "country"=>"Россия",
 "country_code"=>"RU",
 "continent"=>"Европа",
 "subdivision"=>nil,
 "latitude"=>52.731700000000004,
 "longitude"=>41.4433}

city = GeoIP2::locate('GeoLite2-City.mmdb', '77.93.127.33', 'en')
=> {"city"=>"Tambov",
 "country"=>"Russia",
 "country_code"=>"RU",
 "continent"=>"Europe",
 "subdivision"=>nil,
 "latitude"=>52.731700000000004,
 "longitude"=>41.4433}
```

