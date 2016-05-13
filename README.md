# mxml

**mxml** is a C++ parser and layout generator for [MusicXML](http://www.musicxml.com) files.

## Usage

You will need:

* CMake >= 2.6
* LibXML2
* boost (for unit tests)

To build a stand-alone static library:

```
git clone https://github.com/venturemedia/mxml
cd mxml
git submodule update --init
cmake .
make test
```

---

## License

**mxml** is available under the MIT license. See the LICENSE file for more info. Copyright © 2016 Venture Media Labs.
