#define CURL_STATICLIB

// #include <stdio.h>
// #include <string.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <ruby.h>


/* -----------------------------------------------------------------------------
    PROTOTYPES
----------------------------------------------------------------------------- */

// Variable prototypes. This stores the Ruby class information that we will
// bind the download_file and download_files methods to in the FileDownloader 
// class.
VALUE FileDownloader;// = Qnil;

// Class prototype:
// void Init_FileDownloader();

/* -----------------------------------------------------------------------------
    METHODS
----------------------------------------------------------------------------- */

// Download a single file supplied by a Ruby String URL and a filesystem path to
// store the downloaded file to. Only use this method if you are downloading one 
// file, as `download_files` is much more efficient for downloading multiple files.
static VALUE download_file(VALUE self, VALUE rs_url, VALUE rs_path)
{
    
    // Make sure the method arguments are Ruby Strings.
    Check_Type(rs_url, T_STRING);
    Check_Type(rs_path, T_STRING);
    
    // Initialize cURL variables.
    CURL *curl;
    FILE *fp;
    CURLcode res;
    
    // Initialize cURL.
    curl = curl_easy_init();
    
    // Make sure cURL is initialized.
    if (curl) {
    
        // Convert the Ruby URL and path strings into C strings so we can 
        // pass it to cURL.
        char *cs_url, *cs_path;
        cs_url = StringValueCStr(rs_url);
        cs_path = StringValueCStr(rs_path);
        
        // Set general cURL options.
        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); // For debugging...
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL); //write_data
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
        
        // Tell cURL the URL to the file that we want downloaded.
        curl_easy_setopt(curl, CURLOPT_URL, cs_url);
        
        // Open a file for writing and tell cURL to write to it.
        fp = fopen(cs_path, "wb");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        
        // Perform the download.
        res = curl_easy_perform(curl);
        
        // Clean up the cURL session.
        curl_easy_cleanup(curl);
        
        // Close the file after it has been written to.
        fclose(fp);
        
    }
    
    // Return true to Ruby so the function can check whether or not it succeeded.
    return Qtrue;
    
}

// Downloads multiple files that are supplied via a Ruby Array (ra_files). This 
// is much more efficient for downloading multiple files because it only instantiates
// one cURL instance and keeps the connection open while downloading. 
static VALUE download_files(VALUE self, VALUE ra_files)
{
    
    // Make sure the method argument is a Ruby Array.
    Check_Type(ra_files, T_ARRAY);
    
    // Initialize cURL variables.
    CURL *curl;
    FILE *fp;
    CURLcode res;
    
    // Initialize cURL.
    curl = curl_easy_init();
    
    // Make sure cURL is initialized.
    if (curl) {
        
        // Set general cURL options.
        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); // Use for debugging...
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL); //write_data
        
        // Loop through all the files, downloading each one.
        unsigned long int num_files = RARRAY(ra_files)->len;
        long int i;
        for (i = 0; i < num_files; ++i) {
            
            // Initialize and get this item in the Ruby array.
            VALUE ra_item;
            ra_item = rb_ary_entry(ra_files, i);
            
            // Make sure that the item is an array before continuing.
            Check_Type(ra_item, T_ARRAY);
            
            // Get the Ruby URL and file path from the array where we will 
            // download the file from and where we will store it on the filesystem.
            VALUE rs_url, rs_path;
            rs_url = rb_ary_entry(ra_item, 0);
            rs_path = rb_ary_entry(ra_item, 1);
            
            // Convert the Ruby URL and path strings into C strings so we can 
            // pass it to cURL.
            char *cs_url, *cs_path;
            cs_url = StringValueCStr(rs_url);
            cs_path = StringValueCStr(rs_path);
            
            // Open a file for writing and tell cURL to write to it.
            fp = fopen(cs_path, "wb");
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            
            // Tell cURL the URL to the file that we want downloaded.
            curl_easy_setopt(curl, CURLOPT_URL, cs_url);
            
            // Download and write the file to disk.
            res = curl_easy_perform(curl);
            
            // TODO: Get total transfer time to calculate!
            // Get the total time for this transfer and then add it to the 
            // total transfer time.
            // double *cd_trans_time;
            // curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, cd_trans_time);
            // cd_total_trans_time += cd_trans_time;
            
            // Return values to the Ruby block.
            // TODO: Make using a block optional...
            if (!rb_block_given_p()) {
                rb_raise(rb_eArgError, "Expected block for download_files method!");
            }
            rb_yield_values(3, INT2NUM(i), rs_url, rs_path);
            
            // Close the file after the contents have been written to it.
            fclose(fp);
            
            
            // rb_eval_string("files.each{ |x| puts x.inspect }");
            
            // VALUE str, arr;
            // RSTRING(str)->len -> length of the Ruby string
            // RSTRING(str)->ptr -> pointer to string storage
            // RARRAY(arr)->len  -> length of the Ruby array
            // RARRAY(arr)->capa -> capacity of the Ruby array
            // RARRAY(arr)->ptr  -> pointer to array storage
            
            
        }
        
        // Clean up and close the cURL connection after all files have downloaded.
        curl_easy_cleanup(curl);
        
    }
    
    // Return true to Ruby so the function can check whether or not it succeeded.
    return Qtrue;
    
}



/* -----------------------------------------------------------------------------
    CLASSSES
----------------------------------------------------------------------------- */

// This function constructs our Ruby Class and adds the downloading methods to it.
void Init_FileDownloader()
{
    
    FileDownloader = rb_define_module("FileDownloader");
    
    // > FileDownloader::VERSION
    rb_define_const(FileDownloader, "VERSION", rb_str_new2("0.1.0"));
    
    // > FileDownloader.download_file(url, path)
    rb_define_method(FileDownloader, "download_file", download_file, 2);
    
    // > FileDownloader.download_files([])
    rb_define_method(FileDownloader, "download_files", download_files, 1);
    
}


