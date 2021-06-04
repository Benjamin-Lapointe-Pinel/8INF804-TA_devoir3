# Traitement numérique des images - Devoir 3

## Dépendences

Fait sur Linux, Makefile fournit.

- build-essential
- libboost-all-dev

## Construction

```sh
make
```

## Utilisation

```sh
./inpaint -i images/mentuhotep-big-damaged.ppm -m images/big-scratches.ppm -f Y -o y.ppm -a
```

## X

Inspiré de box blur kernel.

## Y

Inspiré de sharpen kernel.

## Références

https://en.wikipedia.org/wiki/Kernel_(image_processing)
