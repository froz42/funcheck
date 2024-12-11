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
        commit = 
          if self ? rev then builtins.substring 0 12 self.rev
          else if self ? dirtyRev then builtins.substring 0 12 (pkgs.lib.removeSuffix "-dirty" self.dirtyRev)
          else "1.1.4";
        funcheck_drv = pkgs.stdenv.mkDerivation (final: {
          pname = "funcheck";
          version = commit;
          src = ./.;
          patches = [
            (
              builtins.toFile
              "remove_fwrite_unlocked.patch"
              ''
                diff --git a/library/srcs/hook/functions/stdio.c b/library/srcs/hook/functions/stdio.c
                index dbb9e2a..2d6b9dd 100644
                --- a/library/srcs/hook/functions/stdio.c
                +++ b/library/srcs/hook/functions/stdio.c
                @@ -104,7 +104,7 @@ DEFINE_HOOK_FUNCTION(size_t, fwrite, EIO, 0, const void *, ptr, size_t, size, si

                 DEFINE_HOOK_FUNCTION(int, fputs_unlocked, EIO, 0, const char *, s, FILE *, stream);
                 DEFINE_HOOK_FUNCTION(size_t, fread_unlocked, EIO, 0, void *, ptr, size_t, size, size_t, nmemb, FILE *, stream);
                -DEFINE_HOOK_FUNCTION(size_t, fwrite_unlocked, EIO, 0, const void *, ptr, size_t, size, size_t, nmemb, FILE *, stream);
                +//DEFINE_HOOK_FUNCTION(size_t, fwrite_unlocked, EIO, 0, const void *, ptr, size_t, size, size_t, nmemb, FILE *, stream);

                 DEFINE_HOOK_FUNCTION(int, fseek, EIO, -1, FILE *, stream, long, offset, int, whence);
                 DEFINE_HOOK_FUNCTION(long, ftell, EIO, -1, FILE *, stream);

              ''
            )
          ];
          nativeBuildInputs = [pkgs.makeWrapper pkgs.minilibx pkgs.gnumake pkgs.xorg.libX11];
          buildInputs = [llvm];
          buildPhase = ''
            ls -la
            export CFLAGS="-Werror -Wextra -Wall -Wno-stringop-truncation \
            -Wno-attributes -Wno-array-parameter -Wno-unused-result \
            -DABSOLUTE_LIBRARY_PATH=\\\"$out/share/funcheck/libfuncheck.so\\\" \
            -g -fPIC -fvisibility=hidden"
            make -C library "CC=$CC" "CFLAGS=$CFLAGS" "VERSION=${final.version}"
            make -C host    "CC=$CC" "CFLAGS=$CFLAGS" "VERSION=${final.version}"
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
