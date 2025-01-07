import os
import argparse
from PIL import Image

# Argument parsing
parser = argparse.ArgumentParser()
parser.add_argument('--indir',  type=str, required=True,   help='Plot directory')
parser.add_argument('--test',   type=bool, required=False, help='Run for only one set of plots')
parser.add_argument('--dryrun', type=bool, required=False, help='Debugging')
args = parser.parse_args()

# Job settings
indir  = args.indir
test   = args.test
dryrun = args.dryrun

outdir = indir
inputplotname = os.listdir(indir)

# Plot dictionary
plotdict = {
    #"numbers": ["nlep", "njet", "nbjet"],  # col=3, row=1
    #"evt": ["HT", "ST", "STvis", "STfrac", "HTMETllpt", "metpt", "metphi"],  # col=4, row=2
    #"lep0": ["lep0_pt", "lep0_eta", "lep0_phi", "lep0_mt", "lep0_iso", "lep0_sip3d"],  # col=3, row=2
    #"lep1": ["lep1_pt", "lep1_eta", "lep1_phi", "lep1_mt", "lep1_iso", "lep1_sip3d"],  # col=3, row=2
    #"dilep": ["dilep_pt", "dilep_eta", "dilep_phi", "dilep_mass", "dilep_mt", "dilep_ptratio"],  # col=3, row=2
    #"angles": ["dilep_deta", "dilep_dphi", "dilep_dR", "dphi_metlep0", "dphi_metlep1", "dphi_metdilep", "dphi_metlep_max", "dphi_metlep_min"],  # col=4, row=2
    #"nnscores": ["nnscore_qcd_vlldmu_200_800", "nnscore_qcd_vlldele_200_800"],
    #"weights": ["2LSS_wt_leptonSF", "2LSS_wt_trig", "2LSS_wt_pileup", "2LSS_wt_bjet", "2LSS_wt_evt"],  # col=3, row=2
    "page1": ["lep0_pt", "lep0_eta", "lep0_phi", "lep0_mt", "lep0_iso", "lep0_sip3d",
              "lep1_pt", "lep1_eta", "lep1_phi", "lep1_mt", "lep1_iso", "lep1_sip3d",
              "dilep_pt", "dilep_eta", "dilep_phi", "dilep_mass", "dilep_mt", "dilep_ptratio"],
    "page2": ["njet", "nbjet", "HT", "ST", "STvis","STfrac",
              "metpt", "metphi","dilep_deta", "dilep_dphi", "dilep_dR","dphi_metdilep",
              "dphi_metlep0", "dphi_metlep1", "dphi_metlep_min", "dphi_metlep_max", "nnscore_qcd_vlldmu_200_800", "nnscore_qcd_vlldele_200_800"]
}

for plottype, plotlist in plotdict.items():
    outfilename = f'combined_{plottype}.png'
    outfilepath = os.path.join(outdir, outfilename)

    plots_to_combine = []
    print(f"\nCombining the following plots as \033[33m{outfilename}\033[0m ...")

    for plot in plotlist:
        match = next((f for f in inputplotname if plot in f), None)
        if match:
            print(plot, end=", ")
            inputplotpath = os.path.join(indir, match)
            plots_to_combine.append(inputplotpath)

    print("\nDone!")

    if not plots_to_combine:
        print(f"No plots found for {plottype}. Skipping...")
        continue

    # Combine the plots into a grid
    images = [Image.open(p) for p in plots_to_combine]
    widths, heights = zip(*(img.size for img in images))

    cols = 4
    if plottype in ["numbers", "lep0", "lep1", "dilep", "weights"]: cols=3
    if plottype in ["nnscores"]: cols=2
    if plottype in ["page1", "page2"]: cols=6
    rows = (len(images) + cols - 1) // cols
    grid_width = max(widths) * cols
    grid_height = max(heights) * rows
    grid_image = Image.new('RGB', (grid_width, grid_height), (255, 255, 255))

    # Paste images into the grid
    for idx, img in enumerate(images):
        x_offset = (idx % cols) * max(widths)
        y_offset = (idx // cols) * max(heights)
        grid_image.paste(img, (x_offset, y_offset))

    if not dryrun:
        grid_image.save(outfilepath)
        print(f"Combined plot saved as \033[32m{outfilepath}\033[0m")

    print("\nDone!")
    if test: break

print("\nAll plots are combined.")
