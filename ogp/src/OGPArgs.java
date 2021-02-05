package ogp;

import java.io.File;

public class OGPArgs {

    private String file;
    private boolean help = false;
    private boolean provers = false;
    private boolean version = false;

    public OGPArgs(String[] args) {
	switch (args.length) {
	case 0:
	    errorMsg(1, "");
	    break;
	case 1:
	    help = args[0].equals("-h") || args[0].equals("--help");
	    provers = args[0].equals("-p") || args[0].equals("--provers");
	    version = args[0].equals("-V") || args[0].equals("--version");
	    if (!help && !provers && !version) {
		testFile(args[0]);
	    }
	    break;
	}
    }

    public String getFile() {
	return this.file;
    }

    public boolean getHelp() {
	return this.help;
    }

    public boolean getProvers() {
	return this.provers;
    }

    public boolean getVersion() {
	return this.version;
    }

    private void testFile(String fileStr) {
	File theFile = new File(fileStr);
	if (!theFile.exists()) {
	    errorMsg(2, fileStr);
	} else if (!theFile.isFile()) {
	    errorMsg(3, fileStr);
	} else if (!theFile.canRead()) {
	    errorMsg(4, fileStr);
	} else {
	    file = fileStr;
	}
    }

    private static void errorMsg(int error, String str) {
	System.err.print("[ERROR] ");
	switch (error) {
	case 1:
	    System.err.print("Incorrect number of arguments.");
	    System.err.println(" Use option '-h' for help.");
	    break;
	case 2:
	    System.err.println("File '" + str + "' does not exist.");
	    break;
	case 3:
	    System.err.println("'" + str + "' is not a file.");
	    break;
	case 4:
	    System.err.println("Cannot read file '" + str + "'.");
	    break;
	}
	System.exit(error);
    }

}
