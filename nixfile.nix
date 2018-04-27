{ pkgs ? import <nixpkgs> {} }:

with pkgs;

stdenv.mkDerivation {
  name = "bonsai";
  buildInputs = [
    stdenv
    cmake
    xorg.libX11
    mesa
  ];
  shellHook = ''
    source ~/.bashrc
  '';
}

