The files in this folder are for use with the Board Manager in the Arduino IDE Version 1.6+

Because the IDE is a bit problematic about updating it's versions of the packages, we have to store them all and make them all available in the json, if we just updated the version code in the json, then the IDE can get really messed up.

Don't even think about having an invalid json structure by the way, the IDE will fail in bad ways, and worse, currently fails to start, unless you delete the offending cached files in your arduino preferences folder.

The `(...)_platform.json` files in this directory get joined togethor and injected into a `package_(...)_index.json` file in the parent directory.
