import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

public class OpenGeometryProver {

    final static int TIMEOUT = 15; // TODO: Make timeout an option
    
    final static String MAJOR_VERSION = "0";
    final static String MINOR_VERSION = "0";
    final static String VERSION = MAJOR_VERSION + "." + MINOR_VERSION;
    
    public static void main(String[] args) {
	if (args.length == 0) {
	    System.err.print("ERROR: Incorrect number of arguments.");
	    System.err.println(" Use option '-h' for help.");
	    System.exit(1);
	}
	if (args[0].equals("-h") || args[0].equals("--help")) {
	    System.out.println("Synopsis: ogp -h | --help");
	    System.out.println("          ogp -p | --provers");
	    System.out.println("          ogp -v | --version");
	    System.out.println("          ogp file");
	    System.out.println("          ogp file prover");
	    System.out.println("          ogp file prover prover_options");
	    System.exit(0);
	}
	if (args[0].equals("-p") || args[0].equals("--provers")) {
	    System.out.println("This option is not yet implemented.");
	    System.exit(0);
	}
	if (args[0].equals("-v") || args[0].equals("--version")) {
	    System.out.println("OpenGeometryProver " + VERSION);
	    System.out.println("Copyright (C) 2021 Nuno Baeta, Pedro Quaresma");
	    System.out.println("Published under GNU GPL, version 3 or later");
	    System.out.println("URL");
	    System.exit(0);
	}
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
	System.exit(0);
    }
    
}
