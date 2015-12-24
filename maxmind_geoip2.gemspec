Gem::Specification.new do |s|
  s.name         = 'maxmind_geoip2'
  s.version      = "0.0.8"

  s.licenses     = ['WTFPL']

  s.authors      = ['Maksim Stepanov']
  s.email        = ['stepanov.ms@mail.ru']

  s.summary      = "A Binding to the GeoIP2 C library"
  s.description  = 'Generic GeoIP2 lookup tool.'
  s.homepage     = "https://github.com/da4nik/geoip2"

  s.files        = `git ls-files`.split("\n")
  s.extensions   = ['ext/maxmind_geoip2/extconf.rb']
  s.require_paths = ["lib"]

  s.add_development_dependency 'minitest',      '~> 5.8.0'
  s.add_development_dependency 'rake',          '~> 10.0'
  s.add_development_dependency 'rake-compiler', '~> 0.9.5'
end
