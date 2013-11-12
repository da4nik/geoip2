# Ruby GeoIP2 Bindings

## Description

Searches city by ip address in local database from [maxmind.com](http://dev.maxmind.com/geoip/geoip2/geolite2/)

## Installation

[libmaxminddb](https://github.com/maxmind/libmaxminddb) must be installed.

```
gem 'geoip2', github: 'da4nik/geoip2'
```

## Usage

### Configuration, can be moved to rails initializer

```ruby
GeoIP2::file '<local_db_file.mmdb>' # default: GeoLite2-City.mmdb
GeoIP2::locale 'ru' # default: 'ru'
```

### Further usage:

Returns nil if nothing found and raises exception if file not opened or not found

```ruby
city = GeoIP2::locate(<ip address>, <optional lang>)

city = GeoIP2::locate '77.93.127.33'
=> {"city"=>"Тамбов",
 "country"=>"Россия",
 "country_code"=>"RU",
 "continent"=>"Европа",
 "latitude"=>52.731700000000004,
 "longitude"=>41.4433}

city = GeoIP2::locate '77.93.127.33', 'en'
=> {"city"=>"Tambov",
 "country"=>"Russia",
 "country_code"=>"RU",
 "continent"=>"Europe",
 "latitude"=>52.731700000000004,
 "longitude"=>41.4433}
```
