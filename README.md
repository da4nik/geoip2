# Ruby GeoIP2 Bindings

## Installation

[libmaxminddb](https://github.com/maxmind/libmaxminddb) must be installed.

```
gem 'geoip2', github: 'da4nik/geoip2'
```

## Usage

```ruby
require 'GeoIP2'
require 'json'

JSON.parse( GeoIP2::locate (<local_db_file.mmdb>, <ip address>) )
```

## Example output

```
{"ru"=> {
  "city"=> {
    "geoname_id"=>"484646",
    "names"=> {
      "de"=>"Tambow",
      "en"=>"Tambov",
      "fr"=>"Tambov",
      "ja"=>"タンボフ",
      "ru"=>"Тамбов",
      "zh-CN"=>"坦波夫"}
  },
  "continent"=> {
    "code"=>"EU",
    "geoname_id"=>"6255148",
    "names"=> {
      "de"=>"Europa",
      "en"=>"Europe",
      "es"=>"Europa",
      "fr"=>"Europe",
      "ja"=>"ヨーロッパ",
      "pt-BR"=>"Europa",
      "ru"=>"Европа",
      "zh-CN"=>"欧洲"}
  },
  "country"=> {
    "geoname_id"=>"2017370",
    "iso_code"=>"RU",
    "names"=> {
      "de"=>"Russland",
      "en"=>"Russia",
      "es"=>"Rusia",
      "fr"=>"Russie",
      "ja"=>"ロシア",
      "pt-BR"=>"Rússia",
      "ru"=>"Россия",
      "zh-CN"=>"俄罗斯"}
  },
  "location"=> {
    "time_zone"=>"Europe/Moscow"
  },
  "registered_country"=> {
    "geoname_id"=>"2017370",
    "iso_code"=>"RU",
    "names"=> {
      "de"=>"Russland",
      "en"=>"Russia",
      "es"=>"Rusia",
      "fr"=>"Russie",
      "ja"=>"ロシア",
      "pt-BR"=>"Rússia",
      "ru"=>"Россия",
      "zh-CN"=>"俄罗斯"}
  },
  "subdivisions"=> {
    "geoname_id"=>"484638",
    "iso_code"=>"TAM",
    "names"=> {
      "en"=>"Tambovskaya Oblast'",
      "ru"=>"Тамбовская область"}
  }
 }
}
```

