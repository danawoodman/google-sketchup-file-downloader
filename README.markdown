# Google SketchUp File Downloader Utility

## Description

A generic and reusable Google SketchUp file downloading utility to be used within Google SketchUp plugins.

The tool is meant to be used by plugin authors (thats you!) in their plugins to facilitate downloading files off of the internet within their tool.

Since SketchUp uses an outdated and stripped down version of Ruby, file downloading is not natively supported within SketchUp. This project aims to solve that issue.

The application consists of a C extension to handle the heavy lifting of file downloading and a Ruby binding that makes using the tool within your plugin simple as possible.


## Features

- **Cross platform** - Works on Mac and PC (Windows XP, Windows Vista and Windows 7) and works with SketchUp 6+.
- **Fast** - Since the core of the tool is implemented in C and utilizes the [libcurl from the cURL project](http://curl.haxx.se/), it is blazingly fast and will download files as fast as the internet connection allows for.
- **Asynchronous** - Since the tool is run in a separate process when it is executed, it runs asynchronously in the background so it doesn't cause SketchUp to freeze while files are downloaded.
- **Easy to implement** - Only takes a few lines of code to integrate into your plugin.
- **Open-source** - The tool is released under an generous open-source MIT license (see `LICENCE` for more info) so you should be able to use it in any of your projects.



## Installation / Integration

This project utilizes [CMake](http://www.cmake.org/) to create cross-platform Makefiles. To create a platform specific Makefile, create a `build` directory and then run CMake:

    cd /path/to/file_downloader/
    mkdir build/ && cd build/
    cmake ../src/
    make

... this will create a Makefile, and then by running `make`, will create a `file_downloader.bundle` file on Mac or a `file_downloader.dll` file on Windows.



## Usage

There are two methods that the Ruby `FileDownloader` class provides:

- `download_file()` which downloads *one file* at a time.
- `download_files()` which downloads *an array of files*.

*If you are just downloading one file, `download_file()` is sufficient, but if you are downloading multiple files, `download_files()` is recommended as it is much faster.*



### Using `download_file()`

The basic usage of the `download_file()` method is as follows:

    # Require the C extension...
    require 'path/to/file_downloader'
    
    # Initialize the FileDownloader tool.
    fd_instance = FileDownloader.new
    
    # Download a file.
    fd_instance.download_file(URL, LOCAL_PATH)

... where `URL` is a string of the URL to the file you want to download and `LOCAL_PATH` is a string of the path where you want to store the file on the local filesystem.

Here is an example of how to use `download_file()`:

    # Require the C extension...
    require 'path/to/file_downloader'
    
    # Initialize the FileDownloader tool.
    fd_instance = FileDownloader.new
    
    # Download a file.
    if fd_instance.download_file("http://www.example.com/somefile.txt", "/path/to/somefile.txt")
        puts "File downloaded successfully!"
    else
        puts "File failed to download!!!"
    end

`download_file()` returns `true` if it succeeds and `false` if the download fails, so you can check if it succeeded, as we did above.



### Using `download_files()`

The basic usage of the `download_files()` method is as follows:

    # Require the C extension...
    require 'path/to/file_downloader'
    
    # Initialize the FileDownloader tool.
    fd_instance = FileDownloader.new
    
    # Download the files.
    fd_instance.download_files(FILE_ARRAY) { |index, url, path| 
        # Code to execute each time a file is downloaded...
    }

... where `FILE_ARRAY` is an array of arrays that contain the URL to the file and the location to store the file on the local filesystem. For example:

    # Require the C extension...
    require 'path/to/file_downloader'
    
    # Create the list of files.
    files = [
        [
            "http://www.example.com/somefile.txt",
            "/path/to/somefile.txt"
        ],
        [
            "http://www.example.com/other_file.rb",
            "/path/to/other_file.rb"
        ],
        [
            "http://www.example.com/one-last-file.zip",
            "/path/to/one-last-file.zip"
        ]
    ]
    
    # Initialize the FileDownloader tool.
    fd_instance = FileDownloader.new
    
    # Download the files.
    if fd_instance.download_files(files) { |index, url, path|
        puts "Downloading file ##{index} from #{url} to #{path}..."
    }
        puts "Files downloaded successfully!"
    else
        puts "Files failed to download!!!"
    end

Just like `download_file()`, `download_files()` returns `true` if it succeeds and `false` if the downloading fails.



## Notes

- When putting a file onto the file system, you must have write permission and the directory into which you will put the download file must exist. You can use Ruby's `Dir.mkdir('/path/to/dir/')` method to create directories as needed.



## To-do

- Get working within SketchUp! Duh.
- Cross-platform testing!
- Create Rakefile...?



## Credits

Copyright &copy; 2011 Dana Woodman <dana@danawoodman.com>



## License

Released under an MIT license. See the `LICENSE` file for more information.


