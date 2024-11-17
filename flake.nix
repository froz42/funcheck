{
  description = "Flake utils demo";

  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (
      system: let
        pkgs = nixpkgs.legacyPackages.${system};
        llvm = pkgs.llvm_18;
        funcheck_drv = pkgs.stdenv.mkDerivation (final: {
          pname = "funcheck";
          version = "1.1.4";
          src = ./.;
          nativeBuildInputs = [pkgs.makeWrapper pkgs.minilibx pkgs.gnumake pkgs.xorg.libX11];
          buildInputs = [llvm];
          buildPhase = ''
            substituteInPlace host/srcs/run/runner.c \
             --replace-fail "../library/libfuncheck.so" ""

            # TODO: remove this when I take time to check wtf is happening

            echo "" >library/srcs/hook/functions/stdio.c
            export CFLAGS="-Werror -Wextra -Wall -Wno-stringop-truncation \
            -Wno-attributes -Wno-array-parameter -Wno-unused-result \
            -DABSOLUTE_LIBRARY_PATH=\\\"$out/share/funcheck/libfuncheck.so\\\" \
            -g -fPIC -fvisibility=hidden"
            make -C library "CC=$CC" "CFLAGS=$CFLAGS" "VERSION=${final.version}-nix"
            make -C host    "CC=$CC" "CFLAGS=$CFLAGS" "VERSION=${final.version}-nix"
          '';
          installPhase = ''
            mkdir -p "$out/bin"
            mkdir -p "$out/share/funcheck"
            cp ./host/funcheck "$out/bin/funcheck"
            cp ./library/libfuncheck.so "$out/share/funcheck/libfuncheck.so"
            wrapProgram $out/bin/funcheck \
              --prefix PATH : ${pkgs.lib.makeBinPath [llvm]}
          '';
        });
      in {
        packages = rec {
          funcheck = default;
          default = funcheck_drv;
        };
        apps = rec {
          funcheck = flake-utils.lib.mkApp {drv = self.packages.${system}.funcheck;};
          default = funcheck;
        };
      }
    );
}
