package ogptest;

import java.io.File;
import java.io.FileNotFoundException;;
import java.util.concurrent.TimeUnit;
import java.util.Scanner;

public class OGPTest {

    final static String MAJOR_VERSION = "0";
    final static String MINOR_VERSION = "2";
    final static String VERSION = MAJOR_VERSION + "." + MINOR_VERSION;
    
    public static void main(String[] args)
	throws FileNotFoundException, InterruptedException {
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

    private static void prove(OGPTestArgs arguments) {
	if (arguments.getVerbose()) {
	    versionMsg();
	    System.out.println("");
	    System.out.println("   File: " + arguments.getFile());
	    System.out.println("Timeout: " + arguments.getTimeout());
	    System.out.println("Verbose: " + arguments.getVerbose());
	    System.out.println("");
	    try {
		Scanner scanFile = new Scanner(new File(arguments.getFile()));
		while (scanFile.hasNextLine()) {
		    System.out.println(scanFile.nextLine());
		}
	    } catch (FileNotFoundException e) {
		e.printStackTrace(System.err);
	    }
	}
	try {
	    TimeUnit.SECONDS.sleep(arguments.getTimeout());
	} catch (InterruptedException e) {
	    e.printStackTrace(System.err);
	}
	if (arguments.getVerbose()) {
	    System.out.println("");
	    System.out.println("Done!");
	}
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

}
