This tool works with common C++ compilers (such as GCC and VC 7.1) to leverage C++ constructs for scripts. It is simply beyond my grasp that one needs 'special script' languages, like Perl, in order to do 'small' tasks.

More info is found at http://blog.davber.com.

Release information is found at CpshVersion1.

### Sample Sessions ###

I.e., one can have this shell file, that goes over all files in a given directory and prints out the big ones:

```
#!/usr/bin/cpsh
path topPath = argv[1]
remove_copy_if (
  directory_iterator(topPath),
  directory_iterator(),
  bind(less<size_t>, bind(file_size,_1), 10000000L),
  ostream_iterator<path>(cout, '\n'));
```

Using ordinary C++ (with the Boost library accessible directly, such as the 'path' type in the filesystem sub namespace)

It also comes with an interactive shell, via the '-i' option, enabling sessions as (the text following '>' is what the user types):

```
> vector<string> names;
> ? sizeof names
< 20
> ? names.size()
< 0
> const char* texts[] = { 'John', 'Greg', 'Bob' };
> copy (texts, texts+3, insert_iterator(names));
> ? names.size();
< 3
> transform (names.begin(), names.end(), \
>>  bind(substr, _1, 1), \
>>  ostream_iterator<string>(cout, ' '));
{ohn reg ob } 
```

This is perfect for learning the language or verifying one's knowledge about complicated constructs.

So, why learn a new scripting language when all the power you need is available in C++, at least as succinctly expressed as with any other language. Oh, did I forget to mention that C++ executables are a bit faster than interpreted Ruby :-)