package ogptest;

// import java.io.File;
// import java.io.FileNotFoundException;;
// import java.util.concurrent.TimeUnit;
// import java.util.Scanner;

public class OGPTest {

    public static void main(String[] args) {
	// throws FileNotFoundException, InterruptedException {
	OGPTestConf configuration = new OGPTestConf();
	OGPTestArgs arguments = new OGPTestArgs(args);
	if (arguments.getHelp()) {
	    helpMsg();
	} else if (arguments.getVersion()) {
	    versionMsg(configuration.getVersion());
	} else {
	    prove(arguments);
	}
	System.exit(0);
    }

    private static void prove(OGPTestArgs arguments) {
	// TODO
    }

    private static void helpMsg() {
	System.out.println("Usage: ogptest [OPTION] [FILE TIMEOUT]");
	System.out.println("where OPTION is one of:");
	System.out.print("    -h | --help                     ");
	System.out.println("prints help message (to be used alone)");
	System.out.print("    -V | --version                  ");
	System.out.println("print OGP version (to be used alone)");
    }

    private static void versionMsg(String version) {	
	System.out.println("OGPTest " + version);
	System.out.println("Copyright (C) 2021 Nuno Baeta");
	System.out.println("Published under GNU GPL, version 3 or later");
	System.out.println("https://github.com/opengeometryprover/OpenGeometryProver");
    }

}
