{
  description = "Old school engine";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let pkgs = import nixpkgs { inherit system; };

      in {

        packages.engine = pkgs.stdenv.mkDerivation rec {
          pname = "engine";
          version = "0.1.0";

          nativeBuildInputs = with pkgs; [
            pkg-config
            git
            meson
            ninja
            vulkan-headers
          ];

          buildInputs = with pkgs; [
            glfw
            vulkan-tools
            vulkan-loader
          ];

          src = builtins.filterSource (path: type:
            let ignoreFiles = [ "flake.nix" "flake.lock" ".gitignore" ".git" "build"];
            in if pkgs.lib.lists.any (p: p == (baseNameOf path))
            ignoreFiles then
              false
            else
              true) ./.;

        };

        devShell = pkgs.mkShell {
          inputsFrom = builtins.attrValues self.packages.${system};

          buildInputs = with pkgs; [ clang-tools ];
        };

        defaultPackage = self.packages.${system}.engine;

      });
}
