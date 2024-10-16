{ pkgs ? import <nixpkgs> { } }:
pkgs.llvmPackages.stdenv.mkDerivation {
  name = "some-name";
  nativeBuildInputs = with pkgs;[ cmake pkg-config ];
  buildInputs = with pkgs;[
	SDL2
	SDL2_ttf
	SDL2_net
	SDL2_gfx
	SDL2_sound
	SDL2_mixer
	SDL2_image
    ocl-icd
    opencl-headers
  ];
} 
