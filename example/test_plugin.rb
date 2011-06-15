# require 'sketchup.rb'

# Get the base path of where this file is located so we can do relative linking.
base_path = File.expand_path(File.dirname(__FILE__))

# Require the Ruby C extension which on Mac is file_downloader.bundle and on 
# Windows is file_downloader.dll.
require File.join(base_path, '../build/file_downloader')


#-------------------------------------------------------------------------------
#    Test the `download_file()` method.
#-------------------------------------------------------------------------------

# Print out to the console that we are beginning our test.
puts "Testing download_file()..."

# Instatiate a new FileDownloader class instance.
dl_1 = FileDownloader.new

# Starts the timer for the download so we can indicate how long it takes to 
# download the file.
t1_1 = Time.now

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
if dl_1.download_file(url, path)
  puts "\tFile downloaded successfully!"
else
  puts "\tFile failed to download!!!"
end

# Finish timer and let us know how long it took to download.
t2_1 = Time.now
delta_1 = t2_1 - t1_1
puts "\tElapsed time: #{delta_1} seconds"


#-------------------------------------------------------------------------------
#    Test the `download_file()` method.
#-------------------------------------------------------------------------------

# Print out to the console that we are beginning our test.
puts "Testing download_files()..."

# Instatiate a new FileDownloader class instance.
dl_2 = FileDownloader.new

# Starts the timer for the download so we can indicate how long it takes to 
# download the file.
t1_2 = Time.now

# Create an array of files to download. This consists of an array of arrays that 
# contain a URl to the file to download (here just Google's hosted jQuery) and 
# the path to save the file to.
files = [
  [
    'https://ajax.googleapis.com/ajax/libs/jquery/1.6.1/jquery.min.js',
    File.join(base_path, 'test_file_1.txt')
  ],
  [
    'https://ajax.googleapis.com/ajax/libs/jquery/1.6.1/jquery.min.js',
    File.join(base_path, 'test_file_2.txt')
  ],
  [
    'https://ajax.googleapis.com/ajax/libs/jquery/1.6.1/jquery.min.js',
    File.join(base_path, 'test_file_3.txt')
  ]
]

# Download the file. This calls the `download_file` method to download a file.
# If you are downloading multiple files, use the `download_files` method instead.
# We wrap the `dowload_file` method in an if/else statment so we can indicate 
# if the operation succeeded or not. This is useful for error checking or 
# providing other options if the download failed.
if dl_2.download_files(files) { |index, url, path|
  puts "\tDownloading file #{index} from #{url} to #{path}"
}
  puts "\tFiles downloaded successfully!"
else
  puts "\tFiles failed to download!!!"
end

# Finish timer and let us know how long it took to download.
t2_2 = Time.now
delta_2 = t2_2 - t1_2
puts "\tElapsed time: #{delta_2} seconds"

