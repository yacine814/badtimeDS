FROM devkitpro/devkitarm:latest

RUN dkp-pacman -Syyu --noconfirm nds-dev
