Gem::Specification.new do |s|
  s.name         = 'geoip2'
  s.version      = "0.0.1"
  s.licenses     = ['WTFPL']

  s.authors      = ['Maksim Stepanov']
  s.email        = ['stepanov.ms@mail.ru']

  s.summary      = "A Binding to the GeoIP C library"
  s.description  = 'Generic GeoIP lookup tool.'
  s.homepage     = "http://github.com"

  s.files        = `git ls-files`.split("\n")
  s.extensions   = ['extconf.rb']
  s.require_path = '.'

  s.add_development_dependency 'rake', '~>10.0'
end