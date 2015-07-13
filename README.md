# Ruby Maxmind GeoIP2 Bindings

Note: This is a fork version of [da4nik/geoip2](https://github.com/da4nik/geoip2) with strong focus on
supporting upstream Ruby and stability.

[![Build Status](https://travis-ci.org/envato/geoip2.svg)](https://travis-ci.org/envato/geoip2)

## Description

Searches city by IP address in local database from [maxmind.com](http://dev.maxmind.com/geoip/geoip2/geolite2/).

It can be used in any standalone Ruby app or in Lotus or in Rails app.

## Installation

[libmaxminddb](https://github.com/maxmind/libmaxminddb) must be installed.

```
gem 'maxmind_geoip2'
```

## Usage

### Configuration

```ruby
MaxmindGeoIP2.file '<local_db_file.mmdb>' # default: GeoLite2-City.mmdb
MaxmindGeoIP2.locale 'ru' # default: 'ru'
```

You could place above codes into a initializer file in Rails, for example `config/initializers/geoip2.rb`.

### Further usage:

Returns nil if nothing found and raises exception if file not opened or not found

```ruby
city = MaxmindGeoIP2.locate(<ip address>, <optional lang>)

city = MaxmindGeoIP2.locate '77.93.127.33'
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

city = MaxmindGeoIP2.locate '77.93.127.33', 'en'
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

## Testing

```
bundle exec rake
```
