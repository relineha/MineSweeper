##########################################################################################################################
#This module was written by Rayan Sud, an undergraduate mentee working for the LZ group during the summer of 2018
##########################################################################################################################

from PIL import Image
from PIL import ImageDraw
import csv
import random

data = open('IdentifiedDust.csv')
csv_reader = list(csv.reader(data, delimiter=','))
pad = 100
small_pad = 3

#base = "/Volumes/LaCie/GridScanData/Bottom2_Scan1/"
base = "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/FinalBottomScan_PostBlowdown/"
hardcoded = "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/FinalBottomScan_PostBlowdown/"

current_cut = 'Pop10_Constrained_FinalBottom'

# for row in csv_reader:
#Select a random subset of pics for visualisation. Comment out and replace with above line to see all pics.
#n_pics = 54
#for i in range(n_pics):


def crop_all():
    for row in csv_reader:
        print (row)
        
    #    row = random.choice(csv_reader)
        file = row[0].replace(hardcoded,"")
        x_cen = float(row[1])
        y_cen = float(row[2])
        x_hw = float(row[3])
        y_hw = float(row[4])
        image = Image.open(base+file)
        left = x_cen-x_hw
        upper = y_cen-y_hw
        right = x_cen+x_hw
        lower = y_cen+y_hw
        cropped_image = image.crop((left-pad,upper-pad,right+pad,lower+pad))
        cropped_image.save('/Users/ryanlinehan/LZ_Local_Work/Grids/GitlabRepo/GridsMacros/AutomatedGridScanning/MineSweeper/CropDusterStuff/CropDusted/'+file.replace('.bmp','')+'dust_x'+str(x_cen)+'_y'+str(y_cen)+'.bmp')
        

def crop_and_bound_all():
    for row in csv_reader:
#        print (row)
        file = row[0].replace(hardcoded,"")
        x_cen = float(row[1])
        y_cen = float(row[2])
        x_hw = float(row[3])
        y_hw = float(row[4])
        
        image = Image.open(base+file)
        draw = ImageDraw.Draw(image)
        left = x_cen-x_hw
        upper = y_cen-y_hw
        right = x_cen+x_hw
        lower = y_cen+y_hw
        draw.rectangle([left-small_pad,
                        upper-small_pad,
                        right+small_pad,
                        lower+small_pad],
                       outline = (255,0,0))
        cropped_image = image.crop((left-pad,
                                    upper-pad,
                                    right+pad,
                                    lower+pad))
#        cropped_image.save('/Users/ryanlinehan/LZ_Local_Work/Grids/GitlabRepo/GridsMacros/AutomatedGridScanning/MineSweeper/CropDusterStuff/CropDusted/'+current_cut+'/'+file.replace('.bmp','')+'dust_x'+str(x_cen)+'_y'+str(y_cen)+'.bmp')
        cropped_image.save('/Users/ryanlinehan/LZ_Local_Work/Grids/GitlabRepo/GridsMacros/AutomatedGridScanning/MineSweeper/CropDusterStuff/CropDusted/'+current_cut+'_'+file.replace('.bmp','')+'dust_x'+str(x_cen)+'_y'+str(y_cen)+'.bmp')
        
        
crop_and_bound_all()
