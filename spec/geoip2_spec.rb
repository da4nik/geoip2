require 'spec_helper'

describe GeoIP2 do
  let(:result) { GeoIP2.locate('74.125.237.168') }

  it 'sets string encodings to utf8' do
    result['country'].encoding.must_equal(Encoding::UTF_8)
  end
end
