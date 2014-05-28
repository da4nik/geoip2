# Ruby GeoIP2 Bindings

## Description

Searches city by ip address in local database from [maxmind.com](http://dev.maxmind.com/geoip/geoip2/geolite2/).

Use it with rails. For now, I think.

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
"city_geoname_id"=>484646,
"country"=>"Россия",
"country_geoname_id"=>2017370,
"country_code"=>"RU",
"continent"=>"Европа",
"continent_code"=>"EU",
"continent_geoname_id"=>6255148,
"subdivision"=>"Тамбовская область",
"subdivision_code"=>"TAM",
"postal_code"=>nil,
"latitude"=>52.731700000000004,
"longitude"=>41.4433,
"time_zone"=>"Europe/Moscow"}

city = GeoIP2::locate '77.93.127.33', 'en'
=> {"city"=>"Tambov",
"city_geoname_id"=>484646,
"country"=>"Russia",
"country_geoname_id"=>2017370,
"country_code"=>"RU",
"continent"=>"Europe",
"continent_code"=>"EU",
"continent_geoname_id"=>6255148,
"subdivision"=>"Tambovskaya Oblast'",
"subdivision_code"=>"TAM",
"postal_code"=>nil,
"latitude"=>52.731700000000004,
"longitude"=>41.4433,
"time_zone"=>"Europe/Moscow"}
```
