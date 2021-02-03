import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

public class OGP {

    final static int TIMEOUT = 15; // TODO: Make timeout an option
    
    final static String MAJOR_VERSION = "0";
    final static String MINOR_VERSION = "0";
    final static String VERSION = MAJOR_VERSION + "." + MINOR_VERSION;
    
    public static void main(String[] args) {
	OGPArgs arguments = new OGPArgs(args);
	if (arguments.getHelp()) {
	    helpMsg();
	} else if (arguments.getVersion()) {
	    versionMsg();
	} else if (arguments.getProvers()) {
	    System.out.println("[TODO] -p, --provers option not yet implemented");
	} else {
	    String fileN = args[0].substring(0, args[0].lastIndexOf('.'));
	    String fileE = args[0].substring(args[0].lastIndexOf('.') + 1);
	    switch (fileE) {
	    case "coqam":
		System.out.println("Calling CoqAM...");
		break;
	    case "fof":
		try {
		    File conjOut = new File(fileN + "_FOF.out");
		    File conjErr = new File(fileN + "_FOF.err");
		    ArrayList<String> command = new ArrayList<String>();
		    command.add("ogptest");
		    if (args.length > 1) {
			for  (int i = 2; i <= args.length; i++) {
			    command.add(args[i - 1]);
			}
		    }
		    Process proc = new ProcessBuilder(command)
			.redirectOutput(conjOut)
			.redirectError(conjErr)
			.start();
		} catch(IOException e) {
		    System.err.println("ERROR: I/O");
		    e.printStackTrace();
		}
		break;
	    case "gcl":
		System.out.println("Calling GCLC...");
		try {
		    File conjOut = new File(fileN + "_GCLC.out");
		    File conjErr = new File(fileN + "_GCLC.err");
		    ArrayList<String> command = new ArrayList<String>();
		    command.add("gclc");
		    command.add(args[0]);
		    if (args.length > 2) {
			for  (int i = 3; i <= args.length; i++) {
			    command.add(args[i - 1]);
			}
		    }
		    System.out.println(command);
		    Process proc = new ProcessBuilder(command)
			.redirectOutput(conjOut)
			.redirectError(conjErr)
			.start();
		} catch(IOException e) {
		    System.err.println("ERROR: I/O");
		    e.printStackTrace();
		}
		break;
	    default:
		System.out.println("ERROR: Unknown extension!");
	    }
	}
	System.exit(0);
    }
	
    private static void helpMsg() {
	System.out.println("Synopsis: ogp -h | --help");
	System.out.println("          ogp -p | --provers");
	System.out.println("          ogp -V | --version");
	System.out.println("          ogp file");
	System.out.println("          ogp file prover");
	System.out.println("          ogp file prover prover_options");
    }

    private static void versionMsg() {
	System.out.println("OpenGeometryProver " + VERSION);
	System.out.println("Copyright (C) 2021 Nuno Baeta, Pedro Quaresma");
	System.out.println("Published under GNU GPL, version 3 or later");
	System.out.println("https://github.com/opengeometryprover/OpenGeometryProver");
    }

}
