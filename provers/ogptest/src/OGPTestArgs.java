package ogptest;

import java.io.File;

public class OGPTestArgs {

    private String conjectureFile;
    private int timeout = 5;
    private boolean help = false;
    private boolean version = false;

    public OGPTestArgs(String[] args) {
	if (args.length == 1) {
	    // ogptest -h | --help
	    help = args[0].equals("-h") || args[0].equals("--help");
	    // ogptest -V | --version
	    version = args[0].equals("-V") || args[0].equals("--version");
	    if (!help && !version) {
		// ogptest [CONJECTURE]
		theConjectureFile(args[0]);
	    }
	} else if (args.length == 2) {
	    // ogptest --timeout=<time> [CONJECTURE]
	    if (!args[0].startsWith("--timeout=")) {
		errorMsg(204, "");
	    }
	    theTimeout(args[0].substring(10));
	    theConjectureFile(args[1]);
	} else if (args.length == 3) {
	    // ogp -t <time> [CONJECTURE]
	    if (!args[0].equals("-t")) {
		errorMsg(204, "");
	    }
	    theTimeout(args[1]);
	    theConjectureFile(args[2]);
	} else {
	    // Incorrect number of arguments
	    errorMsg(206, "");
	}
    }

    public String getFile() {
	return this.conjectureFile;
    }

    public int getTimeout() {
	return this.timeout;
    }

    public boolean getHelp() {
	return this.help;
    }

    public boolean getVersion() {
	return this.version;
    }

    private void theConjectureFile(String file) {
	File theFile = new File(file);
	if (!theFile.exists()) {
	    errorMsg(201, file);
	} else if (!theFile.isFile()) {
	    errorMsg(202, file);
	} else if (!theFile.canRead()) {
	    errorMsg(203, file);
	}
	conjectureFile = file;
    }

    private void theTimeout(String time) {
	try {
	    timeout = Integer.parseUnsignedInt(time);
	} catch (NumberFormatException e) {
	    errorMsg(205, "");
	}
    }

    private static void errorMsg(int error, String msg) {
	System.err.print("[OGPTest ERROR " + error + "] (OGPTestArgs) ");
	switch (error) {
	case 201:
	    System.err.println("File '" + msg + "' does not exist.");
	    break;
	case 202:
	    System.err.println("'" + msg + "' is not a file.");
	    break;
	case 203:
	    System.err.println("Cannot read file '" + msg + "'.");
	    break;
	case 204:
	    System.err.print("Incorrect syntax.");
	    System.err.println(" Use option '-h' for help.");
	    break;
	case 205:
	    System.err.println("Timeout must be a non-negative integer.");
	    break;
	case 206:
	    System.err.print("Incorrect number of arguments.");
	    System.err.println(" Use option '-h' for help.");
	    break;
	}
	System.exit(error);
    }

}
