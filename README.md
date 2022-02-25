# 🐨 Lyptus 🎋

A configurable in-memory binary patcher.

## 🚀 Usage

Set up the Lyptus config with a list of patches to apply, and inject the Lyptus DLL into the target process. Consider using [Koaloader] for automatic injection on process start-up.

[Koaloader]: https://github.com/acidicoala/Koaloader

[Official forum topic](https://cs.rin.ru/forum/viewtopic.php?p=2536753#p2536753)

## ⚙ Configuration

Lyptus comes with a configuration file `Lyptus.json`, which defines patches that will be applied as soon as Lyptus gets injected into the process. The config file
conforms to the standard JSON format. The description of each available option is presented below:

* `logging`: Enables or disables logging into a `Koaloader.log` file. Possible values: `true`, `false` (default).
* `patches`: An array of objects that describe patches that will be applied in the order they were defined. Each object has the following properties:
  * `name`: A string that informally describes the patch. 
  * `pattern`: A hexadecimal string that specifies the pattern Lyptus will use when searching. The string can contain only hex symbols [0-9][A-F][a-f], whitespaces [ ], and wildcards [?]. Keep in mind that 2 hex symbols constitute 1 byte, hence the string must have an even number of characters.
  * `offset`: A byte offset that will be added to the found address when applying the patch.
  * `replacement`: A hexadecimal string that specifies a sequence of bytes to be written at the target address. The string can contain only hex symbols [0-9][A-F][a-f], and whitespaces [ ].
  * `enabled`: A boolean field which enables or disables the patch
  * `required`: A boolean field which determines if Lyptus should crash when a pattern for a given patch is not found or proceed to the next patch.

## 👋 Acknowledgements

This project makes use of the following open source projects:

- [spdlog](https://github.com/gabime/spdlog)
- [JSON for Modern C++](https://github.com/nlohmann/json)

## 📄 License

This software is licensed under [BSD Zero Clause  License], terms of which are available in [LICENSE.txt]

[BSD Zero Clause  License]: https://choosealicense.com/licenses/0bsd/

[LICENSE.txt]: LICENSE.txt
