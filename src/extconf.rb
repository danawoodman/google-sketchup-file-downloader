require 'mkmf'

# Name the extension.
extension_name = 'file_downloader'

# Make sure the cURL library is installed.
have_library("curl")

# Create the header file.
# create_header()

# Create the Makefile.
create_makefile(extension_name)


# # The destination
# dir_config(extension_name)
# dir_config('curl')
# # have_library('curl')

# if find_executable('curl-config')
#   $CFLAGS << " #{`curl-config --cflags`.strip}"
#   $LIBS << " #{`curl-config --libs`.strip}"
#   # ca_bundle_path=`curl-config --ca`.strip
#   # $defs.push( %{-D HAVE_CURL_CONFIG_CA} )
#   # $defs.push( %{-D CURL_CONFIG_CA='#{ca_bundle_path.inspect}'} )
# elsif !have_library('curl') or !have_header('curl/curl.h')
#   fail <<-EOM
#   Can't find libcurl or curl/curl.h
# 
#   Try passing --with-curl-dir or --with-curl-lib and --with-curl-include
#   options to extconf.
#   EOM
# end

