require 'mkmf'
extension_name = 'GeoIP2'

$LDFLAGS << "#{ENV['LDFLAGS']}"
$CFLAGS << "#{ENV['CFLAGS']}"

if have_header('MMDB.h') and have_library('maxminddb')
  create_makefile(extension_name)
else
  abort "you must have mmdb library"
end

