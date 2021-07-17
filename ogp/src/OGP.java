package ogp;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.concurrent.TimeUnit;

public class OGP {

    final static String MAJOR_VERSION = "0";
    final static String MINOR_VERSION = "0";
    final static String VERSION = MAJOR_VERSION + "." + MINOR_VERSION;
    
    public static void main(String[] args) {
	// TODO: Read configuration file
	// File confFile = new File();
	OGPArgs arguments = new OGPArgs(args);
	if (arguments.getHelp()) {
	    helpMsg();
	} else if (arguments.getVersion()) {
	    versionMsg();
	} else if (arguments.getProvers()) {
	    proversList();
	} else {
	    System.out.println("          Timeout: |" + arguments.getTimeout() + "|");
	    System.out.println("    Conjecture id: |" + arguments.getConjectureId() + "|");
	}
	System.exit(0);
    }
	
    private static void helpMsg() {
	System.out.println("Usage: ogp [OPTION] [CONJECTURE [PROVER [PROVER-OPTIONS]]");
	System.out.println("where OPTION is one of:");
	System.out.print("    -h | --help                     ");
	System.out.println("prints help message (to be used alone)");
	System.out.print("    -p | --provers                  ");
	System.out.println("lists avaliable provers (to be used alone)");
	System.out.print("    -V | --version                  ");
	System.out.println("print OGP version (to be used alone)");
	System.out.print("    -t <time> | --timeout=<time>    ");
	System.out.println("sets timeout for an attempt (in seconds)");
    }

    private static void versionMsg() {
	System.out.println("OpenGeometryProver " + VERSION);
	System.out.println("Copyright (C) 2021 Nuno Baeta, Pedro Quaresma");
	System.out.println("Published under GNU GPL, version 3 or later");
	System.out.println("https://github.com/opengeometryprover/OpenGeometryProver");
    }

    private static void proversList() {
	System.out.println("TODO");
    }
    
}
