# Single-cell Variant Calling via Integer Linear Programming (scVILP)
**single-cell Variant-calling by Integer Linear Programming (scVILP)** is a new approach to mutation calling in single cells with Perfect Phylogeny assumption using Integer Linear Programming formulations. scVILP jointly calls mutations for each cell and estimates the Perfect Phylogeny of those cells in the presence of allelic dropouts and missing data. 
It is possible for the user to investigate the violations of infinite-sites assumption on the inference by setting an upper bound on the number of violations 
## Getting started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.
### Requirements
* **[Anaconda](https://docs.anaconda.com/anaconda/install/)** - we recommend installing Anaconda for your OS to manage the required packages
* **[Gurobi](https://www.gurobi.com/gurobi-and-anaconda-for-mac/)** - after installation of Anaconda, you need to install Gurobi optimizer into Anaconda for Integer Linear Programming 
* **PerfectPhy** - to reconstruct the Perfect Phylogeny we have used PerfectPhy package. You can download it from [here](https://csiflabs.cs.ucdavis.edu/~gusfield/software.html) or install the version provided in the scripts directory of this repository
* **[ete3](http://etetoolkit.org)** - install Python package, ete3 into Anaconda by running:
```
conda install -c etetoolkit ete3
```
* **[PAUP]** (http://phylosolutions.com/paup-test/) - to visualize the phylogenetic tree using the Newick output file, download and install PAUP
## Running scVILP
### Inputs
* The input data of scVILP is the output of sequence alignment in [mpileup](http://www.htslib.org/doc/samtools-mpileup.html) format
* At least the number of cells is required, or a file containing the cell names which is optional, in case the cell names are not avaialble, they are named as *cell k* where *k* is the index of the cell
### Filtering the candidate loci
Running it using the number of cells
```
python loci_filter.py -in ovarian.mpileup -out ./output.mpileup -n 370 -ms 2 -nmc 3 
```
Or, using a text file containing the cell names to run this code
```
python loci_filter.py -in ovarian.mpileup -out ./output,mpileup -names cellNames.txt -ms 2 -nmc 3
```
At this step, the user needs to select the loci on which they are performing the analysis. The parameters are as follows:
* *in*: path to the input file
* *out*: path to the output file
* *ms*: at each cell and genomic loci, this parameter requires a minimum of *ms* variant counts 
* *nmc*: at each genomic loci, this parameter requires at least *n* cells to have the minimum number of variants (*m*)

### Running scVILP
Our method generates the phylogenetic tree in Newick format only when there is no violations of infinite-sites assumption. Before running scVILP, please specify the path to the directory of PerfectPhy in scVILP_main.py, the default path to PerfectPhy directory is the same where the scripts are. To run scVILP, enter the following command:
```
python scVILP_main.py -in <path to the mpileup file> -names <path to the cell names>
```
To see the other options:
```
python scVILP_main.py --help
``
