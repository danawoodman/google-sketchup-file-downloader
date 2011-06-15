require 'sketchup.rb'

# Get the base path of where this file is located so we can do relative linking.
base_path = File.expand_path(File.dirname(__FILE__))

# Require the Ruby C extension which on Mac is file_downloader.bundle and on 
# Windows is file_downloader.dll.
require File.join(base_path, 'file_downloader')

# Print out to the console that we are beginning our test.
puts "Testing download_file()..."

# Instatiate a new FileDownloader class instance.
dl = FileDownloader.new

# Starts the timer for the download so we can indicate how long it takes to 
# download the file.
t1 = Time.now

# Set the URL to the file we want to download and the path where we want to store it.
# In this case we are downloading the jQuery javascript library to test if we 
# can connect to an external server. We will save it in this directory as 
# test_file.txt 
url = 'https://ajax.googleapis.com/ajax/libs/jquery/1.6.1/jquery.min.js'
path = File.join(base_path, 'test_file.txt')

# Download the file. This calls the `download_file` method to download a file.
# If you are downloading multiple files, use the `download_files` method instead.
# We wrap the `dowload_file` method in an if/else statment so we can indicate 
# if the operation succeeded or not. This is useful for error checking or 
# providing other options if the download failed.
if dl.download_file(url, path)
  puts "File downloaded successfully!"
else
  puts "File failed to download!!!"
end

# Finish timer and let us know how long it took to download.
t2 = Time.now
delta = t2 - t1
puts "Elapsed time: #{delta} seconds"
