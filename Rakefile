require "rake/extensiontask"

Rake::ExtensionTask.new "geoip2" do |ext|
  ext.lib_dir = "lib/geoip2"
end

desc "Download free IP to City database"
task :download_free_database do
  file = 'GeoLite2-City.mmdb'
  if File.exist?(file)
    puts "File already exists"
  else
    puts "Downloading file"
    `curl http://geolite.maxmind.com/download/geoip/database/GeoLite2-City.mmdb.gz | gzip -d > #{file}`
  end
end