import java.util.concurrent.TimeUnit;

public class OGPTest {

    final static String MAJOR_VERSION = "0";
    final static String MINOR_VERSION = "1";
    final static String VERSION = MAJOR_VERSION + "." + MINOR_VERSION;
    
    public static void main(String[] args) throws InterruptedException {
	OGPTestArgs arguments = new OGPTestArgs(args);
	if (arguments.getHelp()) {
	    helpMsg();
	} else if (arguments.getVersion()) {
	    versionMsg();
	} else {
	    prove(arguments);
	}
	System.exit(0);
    }

    private static void helpMsg() {
	System.out.println("Synopsis: ogptest -h | --help");
	System.out.println("          ogptest -V | --version");
	System.out.print("          ogptest file [");
	System.out.print(" [ -t number | --timeout=number ] |");
	System.out.println(" [ -v | verbose] ]");
    }

    private static void versionMsg() {	
	System.out.println("OGPTest " + VERSION);
	System.out.println("Copyright (C) 2021 Nuno Baeta");
	System.out.println("Published under GNU GPL, version 3 or later");
	System.out.println("https://github.com/opengeometryprover/OpenGeometryProver");
    }

    private static void prove(OGPTestArgs arguments) {
	if (arguments.getVerbose()) {
	    versionMsg();
	    System.out.println("");
	    System.out.println("   File: " + arguments.getFile());
	    System.out.println("Timeout: " + arguments.getTimeout());
	    System.out.println("Verbose: " + arguments.getVerbose());
	}
	try {
	    TimeUnit.SECONDS.sleep(arguments.getTimeout());
	} catch (InterruptedException e) {
	    e.printStackTrace(System.out);
	}
	if (arguments.getVerbose()) {
	    System.out.println("");
	    System.out.println("Done!");
	}
    }

}
