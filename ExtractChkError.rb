#wirks with univariate time series
#In this script, we extract the errors of the checking data for statistical comparison
#ruby ExtractChkError.rb TrnRMSE ChkRMSE
require 'csv'


#Read Training and Checking sets RMSE from the command line. We use the final values read from FinalError.txt
aveTrn=ARGV[0]	
aveChk=ARGV[1]	

puts Float(aveTrn)
puts Float(aveChk)

#trnRMSE=[]
sum=0
count=0
#Open the file the checking errors will be written there. 
infile=File.open("ChkError.txt")
EOF=false
experNo=0
#Read the first two lines *** and empty line

myline=infile.gets
while !EOF do
  
if  (myline).include?("AveErrorBest_Trn")		# if the line has AveErrorBest_Trn, go through the condition otherwise read another line
	
	
	temp=myline[/(|-)(\d+)(\.)(\d+)((e[+-]\d+)?)/]
    	trnRMSE=Float(temp) 
    	
     	if trnRMSE==Float(aveTrn)		#if trnRMSE is equal to our value go through the condition otherwise read another line
     	
     	
      		myline=infile.gets
     		if (myline).include?("FinalError_Chk")
     		
    			temp=myline[/(|-)(\d+)(\.)(\d+)((e[+-]\d+)?)/]
    			chkRMSE=Float(temp)
    			if(chkRMSE==Float(aveChk))	#if chkRMSE is equal to our value go through the condition otherwise read another line
   			 	myline=infile.gets	#pass the line with number of output
   				 myline=infile.gets
   			 	temp=myline.scan(/-?\d+\.\d+e?[+-]?\d*/)	#all the errors are in one line so we read the line and all the errors are saved in an array
   										# "|" is OR operation and "?" is reptetion of 0 or 1, * means reptition of 0 or more
   										#[] characters there are like OR between. 
   										#we work with array here so we cannot use () in the script
   			 	chkERROR=temp
   			 	chkERROR.each{|x|  
   			 	sum=(Float(x)).abs+sum 
   			 	#puts x
   			 	#puts "t"
   			 	#puts sum 
   			 	}
   			 	puts sum
   			 	puts chkERROR.size
   			 	final=sum/(chkERROR.size)
   			 	#puts ave
   			 	#final=Math.sqrt(ave)
   			 	puts final
   			 	
   				CSV.open("ANCFIS_ELM_datapoints_ChkError.csv","w") do |csv|	#save the errors in file
   			 	
   			 		chkERROR.each do |x| 
   			 			csv << [x]
   			 		end
   				 end
   			 	#break		#if we have same training and checking values for some of the set of parameters, just read the first one. 
   			
   			 else
				myline=infile.gets
 			end
 		else
			myline=infile.gets
  		end
  	else
		myline=infile.gets
	end
else
	
	myline=infile.gets
end
	
#	if myline.nil?
#     break
#     end
 
  
end

