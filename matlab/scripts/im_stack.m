% This programs reads in refocused image stacks

function [Ivol] = im_stack(img_dir)

imnames = dir([img_dir '/*.png']);
if isempty(imnames)
    imnames = dir([img_dir '/*.tiff']);
end

imnames = {imnames.name};

for j = 1:length(imnames)

    %Read in image
    Itemp = imread([img_dir,'/', imnames{j}]);
    Ivol(:,:,j) = Itemp;

end

    



