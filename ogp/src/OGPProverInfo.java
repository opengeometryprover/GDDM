package ogp;

public class OGPProverInfo {

    private String id;		// Prover's name
    private String cmd;		// Command line command
    private String ext;		// Conjecture's extension
    private String toFOFCmd;	// ext2FOF command, if any
    private String toExtCmd;	// FOF2ext command, if any
    private String postProcCmd;	// Post-processing command, if any
    private String desc;	// Prover's description

    public OGPProverInfo() {
    }

    public void setId(String id) {
	this.id = id;
    }

    public void setCmd(String cmd) {
	this.cmd = cmd;
    }

    public void setExt(String ext) {
	this.ext = ext;
    }

    public void setToFOFCmd(String toFOFCmd) {
	this.toFOFCmd = toFOFCmd;
    }

    public void setToExtCmd(String toExtCmd) {
	this.toExtCmd = toExtCmd;
    }

    public void setPostProcCmd(String postProcCmd) {
	this.postProcCmd = postProcCmd;
    }

    public void setDesc(String desc) {
	this.desc = desc;
    }

    public String getId() {
	return this.id;
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

    public String getDesc() {
	return this.desc;
    }

}
