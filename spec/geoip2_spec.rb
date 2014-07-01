require 'spec_helper'

describe GeoIP2 do
  describe '.locate' do
    let(:result) { GeoIP2.locate('74.125.237.168') }

    it 'sets string encodings to utf8' do
      result['country'].encoding.must_equal(Encoding::UTF_8)
    end

    it 'has a continent' do
      result['continent'].must_be_instance_of(String)
    end

    it 'has a continent_code' do
      result['continent_code'].must_be_instance_of(String)
    end

    it 'has a continent_geoname_id' do
      result['continent_geoname_id'].must_be_instance_of(Fixnum)
    end

    it 'has the correct continent_geoname_id' do
      result['continent_geoname_id'].must_equal(6255149)
    end

    it 'has a country' do
      result['country'].must_be_instance_of(String)
    end

    it 'has a country_code' do
      result['country_code'].must_be_instance_of(String)
    end

    it 'has a country_geoname_id' do
      result['country_geoname_id'].must_be_instance_of(Fixnum)
    end

    it 'has a city' do
      result['city'].must_be_instance_of(String)
    end

    it 'has a city_geoname_id' do
      result['city_geoname_id'].must_be_instance_of(Fixnum)
    end

    it 'has a subdivision' do
      result['subdivision'].must_be_instance_of(String)
    end

    it 'has a subdivision_code' do
      result['subdivision_code'].must_be_instance_of(String)
    end

    it 'has a subdivision_geoname_id' do
      result['subdivision_geoname_id'].must_be_instance_of(Fixnum)
    end

    it 'has a postal_code' do
      result['postal_code'].must_be_instance_of(String)
    end

    it 'has a latitude' do
      result['latitude'].must_be_instance_of(Float)
    end

    it 'has a longitude' do
      result['longitude'].must_be_instance_of(Float)
    end

    it 'has a time_zone' do
      result['time_zone'].must_be_instance_of(String)
    end
  end
end
