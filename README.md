# standoff 2 external token

usually updated the same day the game updates

## how to use

1. download bluestacks (this specific version):
   [BlueStacks 5.22.130.1019](https://ak-build.bluestacks.com/public/app-player/windows/nxt/5.22.130.1019/03f0020b47ac3b5affd5d7ea53661c44/BlueStacksMicroInstaller_5.22.130.1019_native.exe?filename=BlueStacksInstaller_5.22.130.1019_native_e0175934c4a0a56f1db683585ffd1bcf.exe)

2. connect via ADB and transfer the token file to the Bluestacks window.

3. run the following commands:
```
   /system/xbin/bstk/su
   cp /sdcard/DCIM/SharedFolder/token /data/media/token
   chmod 777 /data/media/token
   /data/media/token
```
