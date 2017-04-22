install:
	adb install .//app/build/outputs/apk/app-debug.apk 

uninstall:
	adb uninstall com.example.charles.demo 

restart:
	#adb shell monkey -p com.example.charles.demo -c android.intent.category.LAUNCHER 1
	adb shell am force-stop com.example.charles.demo
	adb shell am start -n com.example.charles.demo/com.example.charles.demo.MainActivity

restartzy:
	adb shell setprop ctl.restart zygote

hookinstall:
	adb install /Users/charles/AndroidStudioProjects/nativeHook1/app/build/outputs/apk/app-debug.apk

hookuninstall:
	adb uninstall com.example.charles.nativehook1

hookrestart:
	adb shell am force-stop com.example.charles.nativehook1 
	adb shell am start -n com.example.charles.nativehook1
