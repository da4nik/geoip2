require 'bundler/setup'
require 'minitest/autorun'

require 'geoip2'

unless File.exist?('GeoLite2-City.mmdb')
  raise "GeoLite2-City.mmdb doesn't exist. Run rake download_free_database to get it"
end

GeoIP2.file('GeoLite2-City.mmdb')
