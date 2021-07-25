package ogp;

public class OGPProverInfo {

    
    private String cmd;		// Command line command
    private String ext;		// Conjecture's extension
    private String toFOFCmd;	// ext2FOF command, if any
    private String toExtCmd;	// FOF2ext command, if any
    private String postProcCmd;	// Post-processing command, if any
    private String name;	// Prover's name
    private String desc;	// Prover's description

    public OGPProverInfo(String cmd,
			 String ext,
			 String toFOFCmd,
			 String toExtCmd,
			 String postProcCmd,
			 String name;
			 String desc) {
	this.cmd = cmd;
	this.ext = ext;
	this.toFOFCmd = toFOFCmd;
	this.toExtCmd = toExtCmd;
	this.postProcCmd = postProcCmd;
	this.name = name;
	this.desc = desc;
    }

    public String getCmd() {
	return this.cmd;
    }

    public String getExt() {
	return this.ext;
    }

    public String getToFOFCmd() {
	return this.toFOFCmd;
    }

    public String getToExtCmd() {
	return this.toExtCmd;
    }

    public String getPostProcCmd() {
	return this.postProcCmd;
    }

    public String getName() {
	return this.name;
    }

    public String getDesc() {
	return this.desc;
    }

}
