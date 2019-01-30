{ pkgs ? import <nixpkgs> {} }:

with pkgs;

stdenv.mkDerivation {
  name = "bonsai";
  buildInputs = [
    stdenv
    xorg.libX11
    mesa
  ];
  shellHook = ''
    source ~/.bashrc
  '';
}

