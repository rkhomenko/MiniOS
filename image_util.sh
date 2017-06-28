#!/usr/bin/env bash

SCRIPT_NAME=`basename "$0"`

function log_error {
    echo "$SCRIPT_NAME: $1"
}

function log_string {
    echo "$1"
}

function create_or_update_image {
    IMAGE="$1"
    ELF="$2"
    GRUB_CFG=./grub/grub.cfg
    GRUB_IMG=./grub/background.jpg
    CREATE_IMAGE=false

    if ! [[ -f "$IMAGE" ]]; then
        CREATE_IMAGE=true
        dd if=/dev/zero of="$IMAGE" bs=1M count=64 status=none
    fi

    LOOP=`sudo losetup -f --show "$IMAGE"`
    PART_SUF=p1
    LOOP_PART="$LOOP$PART_SUF"

    if $CREATE_IMAGE ; then
        sudo parted $LOOP mklabel msdos 2> /dev/null
        sudo parted $LOOP mkpart primary ext2 32k 100% -a minimal \
                    2> /dev/null
        sudo parted $LOOP set 1 boot on 2> /dev/null
        sudo mkfs.ext2 -q $LOOP_PART
    else
        sudo partx -a $LOOP
    fi

    TMP_DIR=`mktemp -d`
    sudo mount -t ext2 $LOOP_PART "$TMP_DIR"

    if $CREATE_IMAGE ; then
        sudo mkdir -p "$TMP_DIR/boot/grub"
        sudo cp "$GRUB_CFG" "$TMP_DIR/boot/grub"
        sudo cp "$GRUB_IMG" "$TMP_DIR/boot/grub"
        sudo grub-install --boot-directory "$TMP_DIR/boot" \
                          --modules="part_msdos ext2 elf" $LOOP
    fi

    sudo cp "$ELF" "$TMP_DIR/boot"

    sudo umount "$TMP_DIR"
    rm -rf "$TMP_DIR"
    sudo partx -d $LOOP_PART
    sudo losetup -d $LOOP
}

if ! [[ $# -eq 4 ]]; then
    log_error "Bad arguments!"
    log_string \
        "Usage: $SCRIPT_NAME -i /path/to/image -e /path/to/elf]"
    exit 1
fi

IMAGE=
ELF=

while [[ $# -gt 1 ]]
do
    key="$1"
    case $key in
        -i)
            IMAGE="$2"
            shift
            ;;
        -e)
            ELF="$2"
            shift
            ;;
        *)
            log_error "Unknown option!"
            exit 2
            ;;
    esac
    shift
done

if ! [[ -f "$ELF" ]]; then
    log_error "ELF file does not exist!"
    exit 3
fi

if ! [[ -d `dirname "$IMAGE"` ]]; then
    log_error "Path to image does not exist!"
    exit 4
fi

create_or_update_image "$IMAGE" "$ELF"