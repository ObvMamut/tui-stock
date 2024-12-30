{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
    pkgs.ncurses
    pkgs.libyaml
    pkgs.gcc
    pkgs.makeWrapper
  ];

  shellHook = ''
    export LIBRARY_PATH=/usr/local/lib:$LIBRARY_PATH
    export C_INCLUDE_PATH=/usr/local/include:$C_INCLUDE_PATH
  '';
}