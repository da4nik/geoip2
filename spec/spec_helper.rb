require 'bundler/setup'
require 'minitest/autorun'

require 'geoip2'

GeoIP2.file('GeoLite2-City.mmdb')
