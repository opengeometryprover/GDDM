package ogp;

import java.io.File;

public class OGPArgs {

    private int timeout = 15;
    private String conjectureId;
    private String conjectureExt;
    private String proverId;
    private String proverArgs;
    private boolean tgtp = false;
    private boolean help = false;
    private boolean provers = false;
    private boolean version = false;

    public OGPArgs(String[] args, OGPConf configuration) {
	switch (args.length) {
	case 0:
	    // No arguments
	    errorMsg(21, "");
	    break;
	case 1:
	    /*
	     * One of:
	     *     ogp -h | --help
	     *     ogp -p | --provers
	     *     ogp -V | --version
	     *     ogp [CONJECTURE]
	     */
	    help = args[0].equals("-h") || args[0].equals("--help");
	    provers = args[0].equals("-p") || args[0].equals("--provers");
	    version = args[0].equals("-V") || args[0].equals("--version");
	    if (!help && !provers && !version) {
		conjectureInfo(args[0], configuration);
	    }
	    break;
	case 2:
	    /*
	     * One of:
	     *     ogp --timeout=<time> [CONJECTURE]
	     *     ogp [CONJECTURE] [PROVER]
	     */
	    if (args[0].startsWith("--timeout=")) {
		try {
		    timeout = Integer.parseInt(args[0].substring(10));
		    if (timeout < 1) {
			errorMsg(6, "");
		    }
		    conjectureInfo(args[1], configuration);
		} catch (NumberFormatException e) {
		    errorMsg(6, "");
		}
	    } else {
		conjectureInfo(args[0], configuration);
	    }
	    break;
	case 3:
	    /*
	     * One of:
	     *     ogp -t <time> [CONJECTURE]
	     *     ogp --timeout=<time> [CONJECTURE] [PROVER]
	     *     ogp [CONJECTURE] [PROVER] [PROVER-OPTIONS]
	     */
	    break;
	case 4:
	    /*
	     * One of:
	     *     ogp -t <time> [CONJECTURE] [PROVER]
	     *     ogp --timeout=<time> [CONJECTURE] [PROVER] [PROVER-OPTIONS]
	     *     ogp [CONJECTURE] [PROVER] [PROVER-OPTIONS]
	     */
	    break;
	default:
	    /*
	     * One of:
	     *     ogp -t <time> [CONJECTURE] [PROVER] [PROVER-OPTIONS]
	     *     ogp --timeout=<time> [CONJECTURE] [PROVER] [PROVER-OPTIONS]
	     *     ogp [CONJECTURE] [PROVER] [PROVER-OPTIONS]
	     */
	    break;
	}
    }

    public int getTimeout() {
	return this.timeout;
    }
	    
    public String getConjectureId() {
	return this.conjectureId;
    }

    public String getConjectureExt() {
	return this.conjectureExt;
    }

    public String getProverId() {
	return this.proverId;
    }

    public String getProverArgs() {
	return this.proverArgs;
    }

    public boolean getTGTP() {
	return this.tgtp;
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

    private void conjectureInfo(String conjecture, OGPConf configuration) {
	tgtp = conjecture.startsWith("--tgtp=");
	if (tgtp) {
	    conjectureId = conjecture.substring(7);
	    conjectureExt = "";
	    if (conjectureId.isEmpty()) {
		errorMsg(22, "");
	    }
	    // TODO: Check if conjecture exists in TGTP
	} else {
	    File fileConjecture = new File(conjecture);
	    if (!fileConjecture.exists()) {
		errorMsg(23, conjecture);
	    } else if (!fileConjecture.isFile()) {
		errorMsg(24, conjecture);
	    } else if (!fileConjecture.canRead()) {
		errorMsg(25, conjecture);
	    } else {
		conjectureId = conjecture;
		int index = conjecture.lastIndexOf(".");
		if (index == -1) {
		    errorMsg(26, conjecture);
		}
		conjectureExt = conjecture.substring(index + 1);
		if (!configuration.getProversExt().contains(conjectureExt)) {
		    errorMsg(27, conjectureExt);
		}
	    }
	}
    }

    private static void errorMsg(int error, String msg) {
	System.err.print("[ERROR " + error + "] (OGPArgs) ");
	switch (error) {
	case 21:
	    System.err.print("Incorrect number of arguments.");
	    System.err.println(" Use option '-h' for help.");
	    break;
	case 22:
	    System.err.println("Empty TGTP conjecture.");
	    break;
	case 23:
	    System.err.println("File '" + msg + "' does not exist.");
	    break;
	case 24:
	    System.err.println("'" + msg + "' is not a file.");
	    break;
	case 25:
	    System.err.println("Cannot read file '" + msg + "'.");
	    break;
	case 26:
	    System.err.print("Unable to determine the conjecture's format ");
	    System.err.println("from conjecture '" + msg + "'.");
	    break;
	case 27:
	    System.err.println("Unrecognized extension '" + msg + "'.");
	    break;
	}
	System.exit(error);
    }

}
