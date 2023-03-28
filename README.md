<br />
<div align="center">
  <a href="https://github.com/tmatis/funcheck">
    <img src="https://user-images.githubusercontent.com/54767855/227778602-a56ecf02-4d6e-4472-8054-5e2e551eb42e.png" alt="Logo" height="200"/>
  </a>

  <p align="center">
    A tool for checking functions calls return protections.
    <a href="https://github.com/tmatis/funcheck/issues">Report Bug</a>
    ·
    <a href="https://github.com/tmatis/funcheck/issues">Request Feature</a>
  </p>
</div>

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#run-requirements">Run requirements</a></li>
        <li><a href="#quick-install">Quick install</a></li>
        <li><a href="#build-instructions">Build instructions</a></li>
      </ul>
    </li>
    <li>
      <a href="#usage">Usage</a>
      <ul>
        <li><a href="#basic-usage">Basic usage</a></li>
        <li><a href="#options">Options</a></li>
      </ul>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    </li>
  </ol>
</details>

![image](https://user-images.githubusercontent.com/54767855/227782671-b88067e2-76ff-4e77-9067-3f7b937376c4.png)

## About The Project

Funcheck is a tool for checking functions calls return protections. It check if it is protected at runtime by
making each call fail one by one and check if the program doesn't crash and handle the errors.
**This tool only works on Linux for now.**

You can see the list of handled functions and how to add new ones here: [Hooked functions](https://github.com/tmatis/funcheck/wiki/Functions)

## Getting Started

### Run requirements

- A GNU/Linux distribution
- `addr2line` or `llvm-symbolizer` for getting symbolized stack traces (we advise you to use `llvm-symbolizer` since it gives more information)

### Quick install

```bash
bash -c 'bash <(curl -s https://raw.githubusercontent.com/tmatis/funcheck/main/scripts/install.sh)'
```

You can choose to install temporarily, permanently or uninstall the tool.
If you you want to update the tool, you can run the same command.

You can now use the `funcheck` command. (run `funcheck --help` for more information how to use it)

### Build instructions

#### Requirements

- A modern version of `gcc`
- `make`

#### Build

You need to build both the `funcheck` binary and the `libfuncheck.so` library.

##### Clone the repository

```bash
git clone https://github.com/tmatis/funcheck.git
cd funcheck
```

##### Build the `libfuncheck.so` library

```bash
cd library
make
```

##### Build the `funcheck` binary

```bash
cd ../host
make
```

You now have the `funcheck` binary in the `host` directory and the `libfuncheck.so` library in the `library` directory.

You can launch `./funcheck --help` to see how to use it.

You can add the host directory to your `PATH` to be able to use the `funcheck` command from anywhere.

<p align="right">(<a href="#top">back to top</a>)</p>

## Usage

### Basic usage

```bash
funcheck [OPTIONS] ./program [PROGRAM PARAMETERS]
```

### Options

- `-h, --help` : Display the help message
- `-v, --version` : Display the version
- `-t, --test-functions` : Specify the functions to test --test-functions='malloc, calloc, realloc'
- `-i, --ignore-functions` : Specify the functions to ignore --ignore-functions='malloc, calloc, realloc'
- `-j, --json-output` : Output the result in json format
- `-a, --track-allocations`: Track memory allocations
- `-o, --all-output` : Output stdout and stderr of every test
- `-c, --abort-as-crash` : Treat abort() as a crash
- `-b, --complete-backtrace` :  Keep unknowns functions and utils functions in backtrace
- `-s  --show-current-test` : Show the current actually processed by funcheck

<p align="right">(<a href="#top">back to top</a>)</p>

## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b yourlogin/amazingfeature`)
3. Commit your Changes (`git commit -m '<gitmoji> Add some AmazingFeature'`)
4. Push to the Branch (`git push origin yourlogin/amazingfeature`)
5. Open a Pull Request

<p align="right">(<a href="#top">back to top</a>)</p>

## License

Funcheck - A tool for checking functions calls return protections
Copyright (C) 2023  Theo Matis

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

<p align="right">(<a href="#top">back to top</a>)</p>
