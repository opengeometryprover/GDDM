package ogptest;

import java.io.File;

public class OGPTestArgs {

    private String file;
    private int timeout;
    private boolean help = false;
    private boolean version = false;

    public OGPTestArgs(String[] args) {
	if (args.length == 1) {
	    // ogptest -h | --help
	    help = args[0].equals("-h") || args[0].equals("--help");
	    // ogptest -V | --version
	    version = args[0].equals("-V") || args[0].equals("--version");
	    if (!help && !version) {
		errorMsg(202, args[0]);
	    }
	} else if (args.length == 2) {
	    // ogptest [FILE TIMEOUT]
	    file = args[0];
	    File theFile = new File(file);
	    if (!theFile.exists()) {
		errorMsg(203, file);
	    } else if (!theFile.isFile()) {
		errorMsg(204, file);
	    } else if (!theFile.canRead()) {
		errorMsg(205, file);
	    } else {
		try {
		    timeout = Integer.parseUnsignedInt(args[1]);
		} catch (NumberFormatException e) {
		    errorMsg(206, "");
		}
	    }
	} else {
	    // Incorrect number of arguments
	    errorMsg(201, "");
	}
    }

    public String getFile() {
	return this.file;
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

    private static void errorMsg(int error, String msg) {
	System.err.print("[OGPTest ERROR " + error + "] (OGPTestArgs) ");
	switch (error) {
	case 201:
	    System.err.print("Incorrect number of arguments.");
	    System.err.println(" Use option '-h' for help.");
	    break;
	case 202:
	    System.err.println("Unknown option " + msg + ".");
	    break;
	case 203:
	    System.err.println("File '" + msg + "' does not exist.");
	    break;
	case 204:
	    System.err.println("'" + msg + "' is not a file.");
	    break;
	case 205:
	    System.err.println("Cannot read file '" + msg + "'.");
	    break;
	case 206:
	    System.err.println("Timeout must be a positive integer.");
	    break;
	}
	System.exit(error);
    }

}
