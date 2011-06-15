#define CURL_STATICLIB

// #include <stdio.h>
// #include <string.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include "ruby.h"


/* -----------------------------------------------------------------------------
    PROTOTYPES
----------------------------------------------------------------------------- */

// Variable prototypes:
VALUE FileDownloader = Qnil;

// Class prototype:
void Init_file_downloader();

// Method prototypes:
VALUE init(VALUE self);
// VALUE add(VALUE self, VALUE from, VALUE with);
// VALUE subtract(VALUE self, VALUE from, VALUE with);
// static VALUE transfer_time(VALUE self);
static VALUE download_file(VALUE self, VALUE from, VALUE to);
static VALUE download_files(VALUE self, VALUE files);


/* -----------------------------------------------------------------------------
    CLASSSES
----------------------------------------------------------------------------- */

void Init_file_downloader()
{
    FileDownloader = rb_define_class("FileDownloader", rb_cObject);
    // rb_define_method(MyTest, "initialize", init, 0);
    // rb_define_method(MyTest, "add", add, 2);
    // rb_define_method(MyTest, "subtract", subtract, 2);
    // rb_define_method(MyTest, "transfer_time", transfer_time, 0);
    rb_define_method(FileDownloader, "download_file", download_file, 2);
    rb_define_method(FileDownloader, "download_files", download_files, 1);
}


/* -----------------------------------------------------------------------------
    METHODS
----------------------------------------------------------------------------- */

// VALUE init(VALUE self)
// {
//     return self;
// }
// 
// VALUE add(VALUE self, VALUE from, VALUE with)
// {
//     int result = FIX2INT(from) + FIX2INT(with);
//     return INT2NUM(result);
// }
// 
// VALUE subtract(VALUE self, VALUE from, VALUE with)
// {
//     int result = FIX2INT(from) - FIX2INT(with);
//     return INT2NUM(result);
// }

// size_t write_data(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
//     size_t written;
//     written = fwrite(ptr, size, nmemb, stream);
//     return written;
// }

// static VALUE transfer_time(VALUE self)
// {
//     return rb_iv_get(self, "@trans_time");
// }

static VALUE download_file(VALUE self, VALUE rs_url, VALUE rs_path)
{
    
    // Initialize cURL variables.
    CURL *curl;
    FILE *fp;
    CURLcode res;
    
    // printf("URL: %s\n", STR2CSTR(rs_url));
    // printf("PATH: %s\n", STR2CSTR(rs_path));
    
    // char *rs_url = STR2CSTR(rs_from); //"http://sketch.thesourcetank.com/media/uploads/plugins/zorro/releases/0.1/Zorro.rb";
    // 
    // // char outfilename[FILENAME_MAX] = "/Users/dana/Dropbox/Code/sketchup-plugins/file-downloader/Zorro.rb"; //STR2CSTR(to);
    // char rs_path[FILENAME_MAX] = "";
    // strcat(rs_path, STR2CSTR(rs_to));
    // 
    curl = curl_easy_init();
    
    
    // Make sure cURL was initialized.
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
    
    return Qtrue;
    
}

static VALUE download_files(VALUE self, VALUE ra_files)
{
    
    // Make sure the result is an array.
    Check_Type(ra_files, T_ARRAY);
    
    // Initialize the variable to store the total file transfer time.
    double cd_total_trans_time = 0.00;
    
    // Initialize cURL variables.
    CURL *curl;
    FILE *fp;
    CURLcode res;
    
    // Initialize cURL.
    curl = curl_easy_init();
    
    // Make sure cURL is initialized.
    if (curl) {
        
        // Set general cURL options.
        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); // For debugging...
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
        
        // Set the `@trans_time` instance variable with the total download time.
        // rb_iv_set(self, "@trans_time", rb_float_new(cd_total_trans_time));
        
        // Clean up and close the cURL connection after all files have downloaded.
        curl_easy_cleanup(curl);
        
    }
    
    // Return true to Ruby so the function can check whether or not it succeeded.
    return Qtrue;
    
}



















/*

// ORIGINAL CODE:

#define CURL_STATICLIB
#include <stdio.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <string>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int main(void) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char *url = "http://localhost/aaa.txt";
    char outfilename[FILENAME_MAX] = "C:\\bbb.txt";
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return 0;
}

*/






// VALUE subtract(VALUE self)
// {
//     int x = 10;
//     return INT2NUM(x);
//     
//     // CURL *curl;
//     // FILE *fp;
//     // CURLcode res;
//     // char *url = "http://sketch.thesourcetank.com/media/uploads/plugins/zorro/releases/0.1/Zorro.rb";
//     // char outfilename[FILENAME_MAX] = "/Users/dana/Dropbox/Code/sketchup-plugins/file-downloader/Zorro.rb";
//     // // char *url = "http://localhost/aaa.txt";
//     // // char outfilename[FILENAME_MAX] = "C:\\bbb.txt";
//     // curl = curl_easy_init();
//     // if (curl) {
//     //     fp = fopen(outfilename,"wb");
//     //     curl_easy_setopt(curl, CURLOPT_URL, url);
//     //     // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
//     //     curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
//     //     res = curl_easy_perform(curl);
//     //     curl_easy_cleanup(curl);
//     //     fclose(fp);
//     // }
//     // return 0;
//     
// }










// // Prototypes
// VALUE cMyTest = Qnil;
// void Init_MyTest();
// static VALUE t_init(VALUE self);
// 
// // Class initializer.
// void Init_MyTest()
// {
//     cMyTest = rb_define_class("MyTest", rb_cObject);
//     rb_define_method(MyTest, "initialize", t_init, 0);
//     // rb_define_method(MyTest, "add", add, 1);
//     // rb_define_method(MyTest, "subtract", subtract, 1);
// }
// 
// // Class methods.
// static VALUE init(VALUE self)
// {
//     return self;
// }

// static VALUE add(VALUE self, VALUE from, VALUE with)
// {
//     // ERROR CHECKING HERE...
//     return INT2NUM(from + with);
// }
// 
// static VALUE subtract(VALUE self, VALUE from, VALUE with)
// {
//     // ERROR CHECKING HERE...
//     return INT2NUM(from - with);
// }



// // Defining a space for information and references about the module to be stored internally
// VALUE MyTest = Qnil;
// 
// // Prototype for the initialization method - Ruby calls this, not you
// void Init_mytest();
// 
// // Prototype for our method 'test1' - methods are prefixed by 'method_' here
// VALUE method_test1(VALUE self);
// 
// // The initialization method for this module
// void Init_mytest()
// {
//     MyTest = rb_define_module("MyTest");
//     rb_define_method(MyTest, "test1", method_test1, 0);
// }
// 
// // Our 'test1' method.. it simply returns a value of '10' for now.
// VALUE method_test1(VALUE self)
// {
//     int x = 10;
//     return INT2NUM(x);
//     
//     // CURL *curl;
//     // FILE *fp;
//     // CURLcode res;
//     // char *url = "http://sketch.thesourcetank.com/media/uploads/plugins/zorro/releases/0.1/Zorro.rb";
//     // char outfilename[FILENAME_MAX] = "/Users/dana/Dropbox/Code/sketchup-plugins/file-downloader/Zorro.rb";
//     // // char *url = "http://localhost/aaa.txt";
//     // // char outfilename[FILENAME_MAX] = "C:\\bbb.txt";
//     // curl = curl_easy_init();
//     // if (curl) {
//     //     fp = fopen(outfilename,"wb");
//     //     curl_easy_setopt(curl, CURLOPT_URL, url);
//     //     // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
//     //     curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
//     //     res = curl_easy_perform(curl);
//     //     curl_easy_cleanup(curl);
//     //     fclose(fp);
//     // }
//     // return 0;
//     
// }




// #include <stdio.h>
// #include <string.h>
// #include <curl/curl.h>
// #include <curl/types.h>
// #include <curl/easy.h>
// #include "/usr/local/include/curl/curl.h"
// #include "/usr/local/include/curl/types.h"
// #include "/usr/local/include/curl/easy.h"

// #ifdef __arch64__
// #define CURL_SIZEOF_LONG 8
// #else
// #define CURL_SIZEOF_LONG 4
// #endif


// C Datatypes to Ruby objects:
// INT2NUM(int)     -> Fixnum or Bignum
// INT2FIX(int)     -> Fixnum (faster)
// INT2NUM(long or int)     -> Fixnum or Bignum
// INT2FIX(long or int)     -> Fixnum (faster)
// CHR2FIX(char)    -> Fixnum
// rb_str_new2(char *)  -> String
// rb_float_new(double)     -> Float


// class Test
//   def initialize
//     @arr = Array.new
//   end
//   def add(anObject)
//     @arr.push(anObject)
//   end
// end

// #include "ruby.h"
// 
// 
// static VALUE t_init(VALUE self)
// {
//     VALUE arr;
// 
//     arr = rb_ary_new();
//     rb_iv_set(self, "@arr", arr);
//     return self;
// }
// 
// 
// static VALUE t_add(VALUE self, VALUE anObject)
// {
//     VALUE arr;
// 
//     arr = rb_iv_get(self, "@arr");
//     rb_ary_push(arr, anObject);
//     return arr;
// }
// 
// 
// VALUE cTest;
// 
// 
// void Init_Test() {
//     cTest = rb_define_class("Test", rb_cObject);
//     rb_define_method(cTest, "initialize", t_init, 0);
//     rb_define_method(cTest, "add", t_add, 1);
// }









// #define CURL_STATICLIB
// #include <stdio.h>
// // #include <curl/curl.h>
// // #include <curl/types.h>
// // #include <curl/easy.h>
// #include "/usr/local/include/curl/curl.h"
// #include "/usr/local/include/curl/types.h"
// #include "/usr/local/include/curl/easy.h"
// #include <string.h>
// 
// size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
// {
//     size_t written;
//     written = fwrite(ptr, size, nmemb, stream);
//     return written;
// }
// 
// int main(void)
// {
//     CURL *curl;
//     FILE *fp;
//     CURLcode res;
//     char *url = "http://sketch.thesourcetank.com/media/uploads/plugins/zorro/releases/0.1/Zorro.rb";
//     char outfilename[FILENAME_MAX] = "/Users/dana/Dropbox/Code/sketchup-plugins/file-downloader/Zorro.rb";
//     // char *url = "http://localhost/aaa.txt";
//     // char outfilename[FILENAME_MAX] = "C:\\bbb.txt";
//     curl = curl_easy_init();
//     if (curl) {
//         fp = fopen(outfilename,"wb");
//         curl_easy_setopt(curl, CURLOPT_URL, url);
//         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
//         res = curl_easy_perform(curl);
//         curl_easy_cleanup(curl);
//         fclose(fp);
//     }
//     return 0;
// }
